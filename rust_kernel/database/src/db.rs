use crate::db_log;
use crate::entity::user::User;
use crate::entity::user_entity;
use crate::entity::user_entity::user::dsl::*;
use chrono::{Duration, Timelike, Utc};
use diesel::prelude::*;
use diesel::sqlite::SqliteConnection;
use std::error::Error;
use tokio::runtime::Runtime;

pub struct DbManager {
    pub rt: Runtime,
    pub connection: SqliteConnection,
}

impl DbManager {
    pub fn new(url: &str) -> Result<Self, Box<dyn Error>> {
        db_log::log_info("初始化数据库连接...");
        let rt = match Runtime::new() {
            Ok(rt) => rt,
            Err(e) => {
                let error = format!("Failed to create runtime: {}", e);
                db_log::log_error(&error);
                return Err(error.into());
            }
        };

        let conn = match SqliteConnection::establish(url) {
            Ok(conn) => conn,
            Err(e) => {
                let error = format!("Failed to connect to database: {}", e);
                db_log::log_error(&error);
                return Err(error.into());
            }
        };

        db_log::log_info("数据库连接初始化成功...");
        Ok(Self {
            rt,
            connection: conn,
        })
    }

    pub fn is_connected(&mut self) -> Result<bool, Box<dyn Error>> {
        // 对于 SQLite，尝试执行一个简单的查询来检查连接
        match diesel::sql_query("SELECT 1").execute(&mut self.connection) {
            Ok(_) => Ok(true),
            Err(e) => Err(e.into()),
        }
    }

    /// 插入用户
    ///
    /// # Arguments
    /// * `input_user` - Input user
    ///
    /// # Returns
    /// * `Ok(last_insert_id)` - Last insert id
    /// * `Err(error)` - Error
    pub fn insert_user(&mut self, input_user: &User) -> Result<i32, Box<dyn Error>> {
        let tmp_user = input_user.to_owned();

        let tm =match (Utc::now().naive_local() + Duration::hours(8)).with_nanosecond(0) {
            Some(tm) => tm,
            None => return Err("Failed to get current time".into()),
        };

        let new_user = user_entity::AddUser {
            user_name: tmp_user.user_name,
            password: tmp_user.password,
            created_at: tm,
            updated_at:None,
        };
        let result = diesel::insert_into(user)
            .values(&new_user)
            .returning(id)
            .get_result::<i32>(&mut self.connection);
        match result {
            Ok(last_insert_id) => {
                // SQLite 返回受影响的行数
                Ok(last_insert_id)
            }
            Err(e) => Err(e.into()),
        }

    }

    /// 根据用户名获取用户
    ///
    /// # Arguments
    /// * `username` - Username
    ///
    /// # Returns
    /// * `Ok(user)` - User
    /// * `Err(error)` - Error
    pub fn get_user_by_username(&mut self, username: &str) -> Result<User, Box<dyn Error>> {
        let result = user
            .filter(user_name.eq(username))
            .first::<user_entity::UserEntity>(&mut self.connection);

        match result {
            Ok(entity) => {
                Ok(User::new(entity.user_name, entity.password))
            }, 
            Err(e) => {
                let error = format!("Failed to get user by username: {}", e);
                Err(error.into())
            }
        }
    }

    /// 更新用户密码
    ///
    /// # Arguments
    /// * `username` - Username
    /// * `pwd` - Password
    ///
    /// # Returns
    /// * `Ok(affected_rows)` - Affected rows
    /// * `Err(error)` - Error
    pub fn update_user_password(&mut self, username: &str, pwd: &str) -> Result<i32, Box<dyn Error>> {
        let result = diesel::update(user)
            .filter(user_name.eq(username))
            .set(password.eq(pwd))
            .returning(id)
            .get_result::<i32>(&mut self.connection);
        match result {
            Ok(affected_rows) => {
                Ok(affected_rows)
            }
            Err(e) => {
                let error = format!("Failed to update user password: {}", e);
                Err(error.into())
            }
        }
    }

    /// 删除用户
    ///
    /// # Arguments
    /// * `username` - Username
    ///
    /// # Returns
    /// * `Ok(affected_rows)` - Affected rows
    /// * `Err(error)` - Error
    pub fn delete_user(&mut self, username: &str) -> Result<i32, Box<dyn Error>> {
        let result = diesel::delete(user)
            .filter(user_name.eq(username))
            .returning(id)
            .get_result::<i32>(&mut self.connection);
        match result {
            Ok(affected_rows) => {
                Ok(affected_rows)
            }
            Err(e) => {
                let error = format!("Failed to delete user: {}", e);
                Err(error.into())
            }
        }
    }

    /// 获取所有用户
    ///
    /// # Returns
    /// * `Ok(users)` - Users
    /// * `Err(error)` - Error
    pub fn get_all_users(&mut self) -> Result<Vec<User>, Box<dyn Error>> {
        let result = user.load::<user_entity::UserEntity>(&mut self.connection);
        match result {
            Ok(users) => {
                Ok(users.into_iter().map(|entity| User::new(entity.user_name, entity.password)).collect())
            }
            Err(e) => {
                let error = format!("Failed to get all users: {}", e);
                Err(error.into())
            }
        }
    }
}
