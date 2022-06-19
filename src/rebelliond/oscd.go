package main

import (
	"crypto/rand"
	"encoding/base64"
	"fmt"
	"net"
	"os"
	"strconv"
	"time"

	"github.com/hypebeast/go-osc/osc"
)

func getIpPortFromAddr(laddr net.Addr) (string, uint) {
	var lIP string
	var lPort uint

	switch addr := laddr.(type) {
	case *net.UDPAddr:
		lIP = addr.IP.String()
		lPort = uint(addr.Port)
	case *net.TCPAddr:
		lIP = addr.IP.String()
		lPort = uint(addr.Port)
	}

	return lIP, lPort
}

type oscClient struct {
	client   *osc.Client
	lastping int64
	addr     string
	rtt      int64
	rtd      int64
}

func (c *oscClient) Send(message *osc.Message) error {
	return c.client.Send(message)
}

//--- https://stackoverflow.com/a/32351471 -----------
// GenerateRandomBytes returns securely generated random bytes.
// It will return an error if the system's secure random
// number generator fails to function correctly, in which
// case the caller should not continue.
func GenerateRandomBytes(n int) ([]byte, error) {
	b := make([]byte, n)
	_, err := rand.Read(b)
	// Note that err == nil only if we read len(b) bytes.
	if err != nil {
		return nil, err
	}

	return b, nil
}

// GenerateRandomString returns a URL-safe, base64 encoded
// securely generated random string.
func GenerateRandomString(s int) (string, error) {
	b, err := GenerateRandomBytes(s)
	return base64.URLEncoding.EncodeToString(b), err
}

//-----------------------------------------------------

//var clients map[string]*osc.Client
var clients map[string]*oscClient
var client_timeout int64
var client_waitping int64

func rebellionOSCD() {

	fmt.Println("rebellionOSCD")
	clients = make(map[string]*oscClient)
	client_timeout = 10
	client_waitping = 10

	//--- main server, rebellion side
	go func() {
		fmt.Println("starting OSCD goroutine")
		addr := "0.0.0.0:8765"
		server := &osc.Server{}
		conn, err := net.ListenPacket("udp", addr)
		if err != nil {
			fmt.Println("SERVER> Couldn't listen: ", err)
		}
		defer conn.Close()

		fmt.Println("SERVER> Start listening on", addr)
		for {
			packet, err := server.ReceivePacket(conn)
			if err != nil {
				fmt.Println("SERVER> error: " + err.Error())
				os.Exit(1)
			}

			if packet != nil {
				fmt.Println("SERVER> SWITCH")
				switch packet.(type) {
				default:
					fmt.Println("SERVER> Unknown packet type!")

				case *osc.Message:
					omsg := packet.(*osc.Message)

					switch omsg.Address {
					case "/message/address":
						if len(omsg.Arguments) != 2 {
							fmt.Printf("SERVER> Error, /message/address needs two arguments, has %d\n", len(omsg.Arguments))
							continue
						}

						lIP := omsg.Arguments[0]
						lPort := omsg.Arguments[1]
						laddr := lIP.(string) + ":" + strconv.FormatInt(int64(lPort.(int32)), 10)
						if _, ok := clients[laddr]; ok { //we have an entry in our client map, don't allow another request, it may be a DoS
							fmt.Printf("SERVER> ignoring new client addr request: %s\n", laddr)
							continue
						}

						fmt.Printf("SERVER> received new client connection: %s\n", laddr)
						client := osc.NewClient(lIP.(string), int(lPort.(int32)))
						message := osc.NewMessage("/message/address")
						if token, err := GenerateRandomString(32); err != nil {
							fmt.Printf("SERVER> error generating token: %s\n", err)
						} else {
							message.Append(token)
							if err := client.Send(message); err == nil {
								fmt.Printf("Adding client: %s/%s\n", laddr, token)
								clients[token] = &oscClient{
									client: client,
									addr:   laddr,
								}
							}
						}
					case "/message/pong":
						if len(omsg.Arguments) != 2 {
							fmt.Printf("SERVER> Error, /message/address needs 2 arguments")
							continue
						}

						token := omsg.Arguments[0].(string)
						trem := omsg.Arguments[1].(int64)
						tnow := time.Now().UnixMilli()
						fmt.Printf("SERVER> Client token: %s\n", token)
						if client, ok := clients[token]; !ok {
							fmt.Printf("SERVER> Unknown client! %s\n", token)
						} else {
							fmt.Printf("SERVER> client confirmed: %s - giving %d seconds until next ping\n", token, client_waitping)
							client.rtt = tnow - client.lastping
							client.rtd = trem - tnow
							fmt.Printf("RTT: %d RTD: %d\n", client.rtt, client.rtd)

							client.lastping = -client_waitping
						}

					default:
						fmt.Printf("SERVER> -- OSC Message: ")
						osc.PrintMessage(omsg)
					}

				case *osc.Bundle:
					fmt.Println("SERVER> -- OSC Bundle:")
					bundle := packet.(*osc.Bundle)
					for i, message := range bundle.Messages {
						fmt.Printf("SERVER>  -- OSC Message #%d: ", i+1)
						osc.PrintMessage(message)
					}
				}

			}
		}
	}()

	go func() {
		for {

			//ping client death check
			for token, client := range clients {
				tnow := time.Now().UnixMilli()

				if client.lastping == 0 {
					fmt.Printf("SERVER> Trying to send to %s/%s at %d\n", client.addr, token, tnow)

					client.lastping = tnow
					message := osc.NewMessage("/message/ping")
					message.Append(tnow)
					message.Append(client_waitping)
					client.Send(message)
				} else if client.lastping < 0 {
					client.lastping += 1
				} else if client.lastping+(client_timeout*1000) < tnow {
					fmt.Printf("SERVER> client pong missing within %d seconds for %s. Removing\n", client_timeout, client.addr)
					delete(clients, token)
				}
				/*
					if err := client.Send(message); err != nil {
						fmt.Println("Error sending, removing client: %s\n", client.addr)
						delete(clients, key)
					}
				*/
			}

			time.Sleep(1 * time.Second)

		}
	}()

	go func() {
		for {
			rpc := <-reb_cb_ev_chan
			var message *osc.Message

			switch v := rpc.(type) {
			case *RebellionRpcEvent:
				ev := rpc.(*RebellionRpcEvent)

				fmt.Println("G> EVENT: ", ev)
				message = osc.NewMessage("/rebellion/event/" + ev.Event)

				if ev.Event == "device.state" ||
					ev.Event == "BTN_DATA" {
					data := ev.Data.(map[string]interface{})
					for key, val := range data {
						message.Append(key)
						message.Append(val)
					}
				}
				//					message.Append(data["serial"].(string))
				//					message.Append(data["state"].(string))

				//}
			case *RebellionRpcResult:
				result := rpc.(*RebellionRpcResult)
				fmt.Println("G> RESULT: ", result.Id)
				rpcResults[result.Id] = result
			default:
				fmt.Printf("G> I don't know about type %T!\n", v)
			}

			for token, client := range clients {
				fmt.Printf("SERVER> Sending to: %s\n", token)
				client.Send(message)
			}

			/*
				fmt.Printf("!!!!! NEW CB MESSAGE: %s\n", cbmessage)
				for token, client := range clients {
					message := osc.NewMessage("/message/callback")
					message.Append(cbmessage)
					fmt.Printf("SERVER> Sending to: %s\n", token)
					client.Send(message)
				}
			*/
		}

	}()

}
