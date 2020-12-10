package dao

import (
	"fmt"
	"test/micro-cloud/framework"
	"test/micro-cloud/model"
)

type UserDao struct {
}

func (p *UserDao) Insert(user *model.User) int64 {
	result, err := framework.DB.Exec("INSERT INTO user(`username`,`password`,`create_time`) value(?,?,?)", user.Username, user.Password, user.CreateTime)
	if err != nil {
		fmt.Println("Insert error")
		return 0
	}
	id, err := result.LastInsertId()
	if err != nil {
		fmt.Println("Insert error")
		return 0
	}
	return id
}

func (p *UserDao) SelectUserByName(username string) []model.User {
	rows, err := framework.DB.Query("SELECT * FROM user WHERE username = ?", username)
	if err != nil {
		fmt.Println("selectuserbyname error")
		return nil
	}
	var users []model.User
	for rows.Next() {
		var user model.User
		err := rows.Scan(&user.ID, &user.Username, &user.Password, &user.CreateTime)
		if err != nil {
			fmt.Println("selectuserbyname error")
			continue
		}
		users = append(users, user)
	}
	rows.Close()
	return users
}

func (p *UserDao) SelectAllUser() []model.User {
	rows, err := framework.DB.Query("SELECT * FROM user")
	if err != nil {
		fmt.Println("SelectAllUser error")
		return nil
	}
	var users []model.User
	for rows.Next() {
		var user model.User
		err := rows.Scan(&user.ID, &user.Username, &user.Password, &user.CreateTime)
		if err != nil {
			fmt.Println("SelectAllUser error")
			continue
		}
		users = append(users, user)
	}
	rows.Close()
	return users
}
