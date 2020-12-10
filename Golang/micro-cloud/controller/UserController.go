package controller

import (
	"net/http"
	"test/micro-cloud/framework"
	"test/micro-cloud/service"
	"test/micro-cloud/utils"
)

/**
 * r.PostFormValue  : 可以解析 Post/PUT Content-Type=application/x-www-form-urlencoded 或 Content-Type=multipart/form-data
 */

type UserConterller struct {
}

var userService = new(service.UserService)

func (p *UserConterller) Router(router *framework.RouterHandler) {
	router.Router("/register", p.register)
	router.Router("/login", p.login)
	router.Router("/findAll", p.findAll)
}

//POST Content-Type=application/x-www-form-urlencoded
func (p *UserConterller) register(w http.ResponseWriter, r *http.Request) {
	username := r.PostFormValue("username")
	password := r.PostFormValue("password")
	if utils.Empty(username) || utils.Empty(password) {
		framework.ResultFail(w, "username or password can not be empty")
		return
	}
	id := userService.Insert(username, password)
	if id <= 0 {
		framework.ResultFail(w, "register fail")
		return
	}
	framework.ResultOk(w, "register success")
}

//POST Content-Type=application/x-www-form-urlencoded
func (p *UserConterller) login(w http.ResponseWriter, r *http.Request) {
	username := r.PostFormValue("username")
	password := r.PostFormValue("password")
	if utils.Empty(username) || utils.Empty(password) {
		framework.ResultFail(w, "username or password can not be empty")
		return
	}
	users := userService.SelectUserByName(username)
	if len(users) == 0 {
		framework.ResultFail(w, "user does not exist")
		return
	}
	if users[0].Password != password {
		framework.ResultFail(w, "password error")
		return
	}

	framework.ResultOk(w, "login success")
}

// GET/POST
func (p *UserConterller) findAll(w http.ResponseWriter, r *http.Request) {
	users := userService.SelectAllUser()
	framework.ResultJsonOk(w, users)
}
