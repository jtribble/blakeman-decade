[@bs.val] [@bs.scope ("document", "body")]
external addEventListener : (string, {. "keyCode": int} => unit) => unit = "";

[@bs.val] [@bs.scope ("document", "body")]
external removeEventListener : (string, {. "keyCode": int} => unit) => unit =
  "";
