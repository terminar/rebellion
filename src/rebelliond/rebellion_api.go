// Rebellion
//
// File: rebellion_api.go
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
// License: LGPLv3

package main

/*
#cgo LDFLAGS: -lrebellion

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <rebellion.h>

int rebellion_rpc_callback_proxy(rebellion_message_format mf, rebellion_message_type mt, uint8_t *udata, uint32_t len);
*/
import "C"

import (
	"encoding/json"
	"fmt"
	"log"
	"unsafe"
)

type RebellionRpcCommand struct {
	Method string        `json:"method"`
	Params []interface{} `json:"params"`
	Id     uint64        `json:"id,ommitempty"`
}
type RebellionRpcResult struct {
	Result interface{} `json:"result,ommitempty"`
	Id     uint64      `json:"id,ommitempty"`
	Error  interface{} `json:"error,ommitempty"`
}

type RebellionRpcEvent struct {
	Event string      `json:"event"`
	Data  interface{} `json:"data"`
}

type RebellionRpcCallbackFunc func(data interface{}) int

var rebellionRpcCallbackFunc RebellionRpcCallbackFunc

//export rebellion_rpc_callback_proxy
func rebellion_rpc_callback_proxy(mf C.rebellion_message_format,
	mt C.rebellion_message_type,
	udata *C.uint8_t, len C.uint32_t) C.int {

	data := C.GoBytes(unsafe.Pointer(udata), C.int32_t(len))

	if rebellionRpcCallbackFunc == nil {
		return 0
	}

	if mf != C.REBELLION_MF_JSON {
		panic("unsupported message format, please use REBELLION_MF_JSON")
	}

	if mt == C.REBELLION_MT_EV {
		rpcEvent := &RebellionRpcEvent{}
		if err := json.Unmarshal(data, &rpcEvent); err == nil && rpcEvent.Event != "" {
			fmt.Println("G> seems to be event:", rpcEvent.Event)
			return C.int(rebellionRpcCallbackFunc(rpcEvent))
		}
	}

	if mt == C.REBELLION_MT_RES {
		rpcResult := &RebellionRpcResult{}
		if err := json.Unmarshal([]byte(data), &rpcResult); err == nil {
			fmt.Println("G> seems to be result")
			return C.int(rebellionRpcCallbackFunc(rpcResult))
		}
	}
	fmt.Println("G> unknown data type, not converted")

	return -1
}

func RebellionRpc(data interface{}) int {
	var jsonData []byte
	jsonData, err := json.Marshal(data)
	if err != nil {
		log.Println(err)
	}

	if len(jsonData) > 80 {
		fmt.Println("G> RebellionRpc:", string(jsonData[:80]), string("..."))
	} else {
		fmt.Println("G> RebellionRpc:", string(jsonData))
	}

	result := C.rebellion_rpc(
		C.REBELLION_MF_JSON,
		C.REBELLION_MT,
		(*C.uint8_t)(unsafe.Pointer(&jsonData[0])),
		(C.uint32_t)(len(jsonData)))
	return int(result)
}

func Rebellion(callback RebellionRpcCallbackFunc) {
	rebellionRpcCallbackFunc = callback
	C.rebellion((C.rebellion_rpc_callback)(unsafe.Pointer(C.rebellion_rpc_callback_proxy)))
}

func RebellionLoop(looptime uint32) {
	C.rebellion_loop(C.uint32_t(looptime))
}
