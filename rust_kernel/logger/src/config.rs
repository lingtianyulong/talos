use serde::{Deserialize, Serialize};
use std::error::Error;

#[allow(dead_code)]
#[derive(Debug, Clone, Deserialize, Serialize)]
pub struct LoggerConfig {
    pub log_level: String,
    pub save_path: String,
}

impl Default for LoggerConfig {
    fn default() -> Self {
        Self {
            log_level: "info".to_string(),
            save_path: "logs".to_string(),
        }
    }
}

#[allow(dead_code)]
impl LoggerConfig {
    pub fn new(log_level: String, save_path: String) -> Self {
        Self {
            log_level,
            save_path,
        }
    }

    // Convert LoggerConfig to JSON string
    pub fn to_json(&self) -> Result<String, Box<dyn Error>> {
        match serde_json::to_string_pretty(self) {
            Ok(json) => Ok(json),
            Err(e) => {
                let err_info = format!("Failed to convert LoggerConfig to JSON: {}", e);
                return Err(err_info.into());
            }
        }
    }

    // Convert JSON string to LoggerConfig
    pub fn from_json(json: &str) -> Result<Self, Box<dyn Error>> {
        match serde_json::from_str(json) {
            Ok(config) => Ok(config),
            Err(e) => {
                let err_info = format!("Failed to convert JSON to LoggerConfig: {}", e);
                return Err(err_info.into());
            }
        }
    }
}
