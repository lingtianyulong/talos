use crate::config::LoggerConfig;
use flexi_logger::{Cleanup, Criterion, Logger, LoggerHandle, Naming, WriteMode};
use std::error::Error;
use std::fs::File;
use std::io::Read;
use std::path::Path;
use std::sync::{Once, OnceLock};

static INIT_LOG: Once = Once::new();
static LOGGER_HANDLE: OnceLock<LoggerHandle> = OnceLock::new();

fn my_log_format(
    w: &mut dyn std::io::Write,
    now: &mut flexi_logger::DeferredNow,
    record: &log::Record,
) -> std::io::Result<()> {
    write!(
        w,
        "{} [{}] - {}",
        now.now().format("%Y-%m-%d %H:%M:%S%.3f"),
        record.level(),
        &record.args()
    )
}

pub fn init_log(cfg_file: &str) -> Result<bool, Box<dyn Error>> {
    let p = Path::new(cfg_file);
    let is_json = match p.extension().map(|ext| ext == "json") {
        Some(true) => true,
        _ => false,
    };

    let mut config = LoggerConfig::default();

    if is_json {
        let mut file = match File::open(cfg_file) {
            Ok(file) => file,
            Err(e) => {
                return Err(
                    format!("Failed to open logger config file {}: {}", cfg_file, e).into(),
                );
            }
        };

        let mut contents = String::new();
        let size = match file.read_to_string(&mut contents) {
            Ok(sz) => sz,
            Err(e) => {
                return Err(
                    format!("Failed to read logger config file {}: {}", cfg_file, e).into(),
                );
            }
        };

        // 如果没有读取到内容，则返回错误
        if size == 0 {
            return Err(format!("Logger config file {}: is empty", cfg_file).into());
        }

        config = match LoggerConfig::from_json(&contents) {
            Ok(cfg) => cfg,
            Err(e) => {
                return Err(
                    format!("Failed to parse logger config file {}: {}", cfg_file, e).into(),
                );
            }
        };
    }

    let res = std::panic::catch_unwind(|| {
        let is_async = true;
        let write_mode = if is_async {
            WriteMode::Async
        } else {
            WriteMode::BufferAndFlush
        };

        INIT_LOG.call_once(|| {
            let builder = match Logger::try_with_env_or_str(&config.log_level) {
                Ok(builder) => builder,
                Err(e) => {
                    panic!("Failed to create logger: {}", e);
                }
            };

            let handle = match builder
                .log_to_file(flexi_logger::FileSpec::default().directory(&config.save_path))
                .rotate(
                    Criterion::Size(10_000_000),
                    Naming::Timestamps,
                    Cleanup::KeepLogFiles(5),
                )
                .write_mode(write_mode)
                .format(my_log_format)
                .start()
            {
                Ok(handle) => handle,
                Err(e) => {
                    panic!("Failed to create logger: {}", e);
                }
            };

            let _ = LOGGER_HANDLE.set(handle);
        });
    });

    return Ok(res.is_ok());
}

pub fn trace(msg: &str) {
    log::trace!("{}", msg);
}

pub fn debug(msg: &str) {
    log::debug!("{}", msg);
}

pub fn info(msg: &str) {
    log::info!("{}", msg);
}

pub fn warn(msg: &str) {
    log::warn!("{}", msg);
}

pub fn error(msg: &str) {
    log::error!("{}", msg);
}

pub fn flush() {
    if let Some(handle) = LOGGER_HANDLE.get() {
        handle.flush();
    }
}

pub fn shutdown_log() {
    if let Some(handle) = LOGGER_HANDLE.get() {
        handle.flush();
        std::thread::sleep(std::time::Duration::from_millis(100)); // 等待100毫秒，让日志写入文件
        handle.shutdown();
    }
}
