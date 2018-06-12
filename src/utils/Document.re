[@bs.val] [@bs.scope "window"] external addEventListener : unit => unit = "";

[@bs.val] [@bs.scope ("document", "activeElement")]
external blur : unit => unit = "";

blur();
