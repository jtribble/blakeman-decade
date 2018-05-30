type t;

[@bs.new] external init : string => t = "Audio";

[@bs.send] external play : t => unit = "";

[@bs.send] external pause : t => unit = "";

[@bs.set] external setCurrentTime : (t, float) => unit = "currentTime";
