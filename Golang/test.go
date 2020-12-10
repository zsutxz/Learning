package main

import (
	"io"
	"net/http"
)

func helloHandler(w http.ResponseWriter, req *http.Request) {
	io.WriteString(w, "hello, world1111!\n")
}

// func main() {
// 	http.HandleFunc("/", helloHandler)
// 	http.ListenAndServe(":12345", nil)
// }
