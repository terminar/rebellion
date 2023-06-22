
//mod rebellion_bindings;
use rubelliond::*;
use std::ffi::CString;
use std::process::exit;


#[allow(non_camel_case_types)]
pub type size_t = i32;

#[inline]
#[allow(clippy::not_unsafe_ptr_arg_deref)]
pub fn from_char_buff<'a>(ptr: *const u8, len: size_t) -> Option<&'a str> {
    // SAFETY: We need to make sure that pointer is not null.
    if ptr.is_null() {
        None
    } else {
        unsafe {
            let buff = std::slice::from_raw_parts(ptr, len as usize);
            // SAFETY: Using the standard from_utf8 function, we ensure that the sequence is a valid string or NONE. 
            // In case of invalid sequence of strings the function returns None.
            // Considering that the pointer is also not null there is no scenario under which the return value of this function won't be a proper string.
            // There is no chance for an undefined behaviour.
            // Therefor the return value of this function is both safe and sound.
            // As a consequence not_unsafe_ptr_arg_deref warning is not relevant and has to be supressed.
            std::str::from_utf8(buff).ok()
        }
    }
}


#[allow(unused_variables)]
extern "C" fn cbfunc(
    mf: rebellion_message_format, 
    mt: rebellion_message_type, 
    data: *const u8, 
    len: u32) -> i32 {

    println!("rubelliond> Called from C");

    if mf != rebellion_message_format_REBELLION_MF_JSON {
        println!("rubelliond> unknown message format given");
        return 0;
    }
/*
    if mt != rebellion_message_type_REBELLION_MT_EV {
        println!("rubelliond> only EV message type supported at the moment");
        return 0;
    }
*/
    let l : i32 = len as i32;
    let jsonstring = from_char_buff(data,l);

    //let pjson = json::parse(jsonstring);
    let pjson: serde_json::Value = serde_json::from_str(jsonstring.unwrap()).expect("json event or result expected");
    println!("\n\n\n{:?}\n\n\n", pjson);
    if pjson["error"] != serde_json::json!(null) {
        println!("{}", pjson["error"]);
    }

    return 0;
}

fn main() {

    unsafe {
        rebellion(Some(cbfunc));

        let jsonrpc = r#"{
            "method": "rpc",
            "params": ["foo", "bar"],
            "id": 1
        }"#;
        println!("{}",jsonrpc);

        let c_str = CString::new(jsonrpc).unwrap();
    
        rebellion_rpc(
            rebellion_message_format_REBELLION_MF_JSON,
            rebellion_message_type_REBELLION_MT_REQ,
            c_str.as_ptr() as *const u8,
            jsonrpc.len() as u32
        );

        rebellion_loop(0);
    }
}