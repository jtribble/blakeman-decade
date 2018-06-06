[@bs.val] [@bs.scope "window"] external innerHeight : float = "";

[@bs.val] [@bs.scope "window"] external innerWidth : float = "";

[@bs.val] [@bs.scope "window"] external pageYOffset : float = "";

[@bs.val] [@bs.scope "window"]
external addEventListener : (string, unit => unit, bool) => unit = "";

[@bs.val] [@bs.scope "window"]
external removeEventListener : (string, unit => unit, bool) => unit = "";

[@bs.val] external setInterval : (unit => unit, float) => int = "";

[@bs.val] external clearInterval : int => float = "";
