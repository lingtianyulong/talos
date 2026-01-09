use chrono::NaiveDateTime;
use diesel::prelude::*;

diesel::table! {
    user (id) {
        id -> Integer,
        user_name -> Text,
        password -> Text,
        created_at -> Timestamp,
        updated_at -> Nullable<Timestamp>,      // 该字段可为空
    }
}

/// UserEntity 用户实体类, 用于表示数据库中的用户表
/// 使用 diesel 宏定义用户实体类, 用于表示数据库中的用户表
///
/// # Fields
/// * `id` - User ID (primary key, auto increment)
/// * `user_name` - User name
/// * `password` - User password
/// * `created_at` - Created at
/// * `updated_at` - Updated at (nullable)
#[derive(Debug, Clone, PartialEq, Queryable, Selectable, AsChangeset)]
#[diesel(table_name = user)]
pub struct UserEntity {
    pub id: i32,
    pub user_name: String,
    pub password: String,
    pub created_at: NaiveDateTime,
    pub updated_at: Option<NaiveDateTime>,
}

/// AddUser 用于插入新用户
///
/// # Fields
/// * `user_name` - User name
/// * `password` - User password
/// * `created_at` - Created at
/// * `updated_at` - Updated at (nullable)
#[derive(Insertable)]
#[diesel(table_name = user)]
pub struct AddUser {
    pub user_name: String,
    pub password: String,
    pub created_at: NaiveDateTime,
    pub updated_at: Option<NaiveDateTime>,
}