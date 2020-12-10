package service

import (
	"test/micro-cloud/dao"
	"test/micro-cloud/model"
	"time"
)

type UserService struct {
}

var userDao = new(dao.UserDao)

func (p *UserService) Insert(username, password string) int64 {
	return userDao.Insert(&model.User{Username: username, Password: password, CreateTime: time.Now()})
}

func (p *UserService) SelectUserByName(username string) []model.User {
	return userDao.SelectUserByName(username)
}

func (p *UserService) SelectAllUser() []model.User {
	return userDao.SelectAllUser()
}
