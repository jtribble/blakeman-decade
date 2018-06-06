type t;

[@bs.new] external init : string => t = "Audio";

[@bs.send] external play : t => unit = "";

[@bs.send] external pause : t => unit = "";

[@bs.get] external getVolume : t => float = "volume";

[@bs.set] external setCurrentTime : (t, float) => unit = "currentTime";

[@bs.set] external setVolume : (t, float) => unit = "volume";

let mute = a => setVolume(a, 0.0);

let unMute = a => setVolume(a, 1.0);

let isMuted = a => a |> getVolume |> (v => v == 0.0);

Js.log(init("") |> isMuted);
