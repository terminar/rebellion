// Rebellion
//
// File: rebellion.go
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

package main

import (
	"bufio"
	"fmt"
	"image"
	"image/png"
	"math/rand"
	"os"
	"os/signal"
)

type any = interface{}

var reb_cb_ev_chan chan interface{}

var rpcResults map[uint64]*RebellionRpcResult = make(map[uint64]*RebellionRpcResult)
var rpcReqCnt uint64 = 0
var testState uint32 = 0
var devSerial string

//var reb_cb_ev_chan chan interface{}

func toRGB565(r, g, b uint32) uint16 {
	// RRRRRGGGGGGBBBBB
	return uint16((r & 0xF800) +
		((g & 0xFC00) >> 5) +
		((b & 0xF800) >> 11))
}

func getRebellionImage() image.Image {
	existingImageFile, err := os.Open("rebellion-480x272.png")
	if err != nil {
		panic(err)
	}
	defer existingImageFile.Close()

	existingImageFile.Seek(0, 0)
	loadedImage, err := png.Decode(existingImageFile)
	if err != nil {
		panic(err)
	}
	return loadedImage
}

func RpcCallback(rpc interface{}) int {
	fmt.Println("G> === RpcCallback START ====================================")
	switch v := rpc.(type) {
	case *RebellionRpcEvent:
		ev := rpc.(*RebellionRpcEvent)

		fmt.Println("G> EVENT: ", ev)
		//reb_cb_ev_chan <- "/message/EV/" + ev.Event
		//reb_cb_ev_chan <- rpc
		//fmt.Println("G> set reb_cb_ev_chan")

		if ev.Event == "device.state" {
			data := ev.Data.(map[string]interface{})
			if data["state"] == "ON" {
				fmt.Println("G> Got device.state, setting testState = 2")
				devSerial = data["serial"].(string)
				testState = 2
			} else {
				devSerial = ""
			}
		}
	case *RebellionRpcResult:
		result := rpc.(*RebellionRpcResult)
		fmt.Println("G> RESULT: ", result.Id)
		rpcResults[result.Id] = result
	default:
		fmt.Printf("G> I don't know about type %T!\n", v)
	}

	fmt.Println("G> --- RpcCallback END --------------------------------------")
	return 0
}

func rpcRequest(rpc *RebellionRpcCommand) uint64 {
	rpcReqCnt = rpcReqCnt + 1
	rpc.Id = rpcReqCnt

	if rpc.Params == nil {
		rpc.Params = []interface{}{}
	}
	RebellionRpc(rpc)
	return rpcReqCnt
}

func rpcResult(id uint64) *RebellionRpcResult {
	if id <= 0 {
		return nil
	}

	if res, found := rpcResults[id]; found {
		fmt.Println("G> found result, delete from map")
		delete(rpcResults, id)
		return res
	}
	return nil
}

func rebellionTests() {
	fmt.Println("-------------------------------")

	for {
		switch testState {
		case 0: //first iteration, just increase
			fmt.Println(("G> Calling rpc"))
			if result := rpcResult(rpcRequest(&RebellionRpcCommand{
				Method: "rpc",
			})); result != nil {
				fmt.Println("GOT 'rpc' RESULT result!: ", result)
			}
			testState++

		case 1:
			fmt.Println(("G> Calling rebellion.getDevices"))
			if result := rpcResult(rpcRequest(&RebellionRpcCommand{
				Method: "rebellion.getDevices",
				Params: []interface{}{},
			})); result != nil {
				fmt.Println("G> 'getDevices' Result: ", result)
			}
			testState++

		case 2:
			fmt.Println(("G> Calling rebellion.getInstances"))
			if result := rpcResult(rpcRequest(&RebellionRpcCommand{
				Method: "rebellion.getInstances",
				Params: []interface{}{},
			})); result != nil {
				fmt.Println("G> 'getInstances' Result: ", result)

				//result: [] {name, device}
				if res, ok := result.Result.([]interface{}); ok && len(res) > 0 {
					data := res[0].(map[string]interface{})
					devSerial = data["name"].(string)
				} else {
					testState = 7 //jump to end of teststate, no device instance found
				}

			}
			testState++
		case 3:
			if devSerial != "" {
				fmt.Println(("G> Calling rebellion.sendLedData (random pad 1)"))
				if result := rpcResult(rpcRequest(&RebellionRpcCommand{
					Method: "rebellion.sendLedData",
					Params: []interface{}{devSerial, rand.Intn(16) + 88, rand.Intn(16), rand.Intn(4)},
				})); result != nil {
					fmt.Println("G> 'sendLedData' Result: ", result)
				}
			}
			testState++
		case 4:
			if devSerial != "" {
				fmt.Println(("G> Calling rebellion.sendLedData (random pad 2)"))
				if result := rpcResult(rpcRequest(&RebellionRpcCommand{
					Method: "rebellion.sendLedData",
					Params: []interface{}{devSerial, rand.Intn(16) + 88, rand.Intn(16), rand.Intn(4)},
				})); result != nil {
					fmt.Println("G> 'sendLedData' Result: ", result)
				}
			}
			testState++
		case 5:
			if devSerial != "" {
				fmt.Println(("G> Calling rebellion.sendDataToDisplay (rgb full color white)"))
				const maxpx = 272 * 480
				data := [maxpx]uint16{}
				for i := 0; i < maxpx; i++ {
					//c := toRGB565(0xffff, 0x0, 0x0)
					//c := toRGB565(0x0, 0xffff, 0x0)
					//c := toRGB565(0x0, 0x0, 0xffff)
					c := toRGB565(0xffff, 0xffff, 0xffff)
					data[i] = c
				}

				if result := rpcResult(rpcRequest(&RebellionRpcCommand{
					Method: "rebellion.sendDataToDisplay",
					Params: []interface{}{devSerial, 0, data},
				})); result != nil {
					fmt.Println("G> 'sendDataToDisplay' 0 finished")
				}
			}
			testState++
		case 6:
			if devSerial != "" {
				fmt.Println(("G> Calling rebellion.sendDataToDisplay (load Rebellion logo image from png)"))

				// const maxX = 480
				// const maxY = 272
				// data := [maxY][maxX]uint16{}
				// col := 0
				// for y := 0; y < maxY; y++ {
				// 	for x := 0; x < maxX; x++ {
				// 		var c uint16 = 0
				// 		switch col {
				// 		case 0:
				// 			c = toRGB565(0xffff, 0x0, 0x0)
				// 		case 1:
				// 			c = toRGB565(0x0, 0xffff, 0x0)
				// 		case 2:
				// 			c = toRGB565(0x0, 0x0, 0xffff)
				// 		}

				// 		data[y][x] = c

				// 		if col >= 2 {
				// 			col = 0
				// 		} else {
				// 			col++
				// 		}

				// 	}
				// }

				img := getRebellionImage()
				maxX := img.Bounds().Dx()
				maxY := img.Bounds().Dy()
				data := make([][]uint16, maxY)

				for i := range data {
					data[i] = make([]uint16, maxX)
				}

				for y := 0; y < maxY; y++ {
					for x := 0; x < maxX; x++ {
						r, g, b, _ := img.At(x, y).RGBA()
						data[y][x] = toRGB565(r, g, b)

					}
				}

				if result := rpcResult(rpcRequest(&RebellionRpcCommand{
					Method: "rebellion.sendDataToDisplay",
					Params: []interface{}{devSerial, 1, data},
				})); result != nil {
					fmt.Println("G> 'sendDataToDisplay' 1 finished")
				}

			}

			testState++
		}

	}

}

func stdin_reader() {
	fmt.Println("### Welcome to Rebellion OSC server demo")
	fmt.Println("Press \"q\" to exit")
	//-- main wait
	reader := bufio.NewReader(os.Stdin)

	for {
		c, err := reader.ReadByte()
		if err != nil {
			os.Exit(0)
		}

		if c == 'q' {
			os.Exit(0)
		}
	}
}

/*
func init() {
	//I believe runtime.LockOSThread() is necessary if you are creating a library binding from C code which uses thread-local storage.
	//Otherwise, just let the scheduler multiplex the goroutines for you.
	//And note that runtime.LockOSThread() only prevents other goroutines from running in that thread until you
	//call runtime.UnlockOSThread().

	//https://github.com/golang/go/wiki/LockOSThread

	runtime.LockOSThread()

}
*/

func main() {

	//runtime.GOMAXPROCS(1)

	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs)
	//  16    SIGURG   discard signal urgent condition present on socket
	go func() {
		for {
			sig := <-sigs
			fmt.Println()
			fmt.Println(sig)
		}
	}()

	//reb_cb_ev_chan = make(chan interface{}, 1024)

	//	go rebellionOSCD()
	/*
		go func() {
			for {
				fmt.Println("!!!!! getting reb_cb_ev_chan message")
				message := <-reb_cb_ev_chan
				fmt.Printf("!!!!! NEW CB MESSAGE: %s\n", message)
			}
		}()
	*/

	Rebellion(RpcCallback)
	fmt.Println("========> Starting RebellionLoop")
	for {
		RebellionLoop(10)
	}

	stdin_reader()
}
