type song = {
  .
  "artist": string,
  "duration": float,
  "song": string,
};

[@bs.val] [@bs.module]
external songMetadata : Js.Dict.t(Js.Array.t(song)) =
  "../constants/song-metadata.json";

let getSongPath = song => "/assets/songs/" ++ song##song ++ ".mp3";

let getSong = (~year, ~duration) =>
  songMetadata
  |. Js.Dict.get(year)
  |. Belt.Option.map(songs =>
       (mod_float(duration, songs[0]##duration +. songs[1]##duration), songs)
     )
  |. Belt.Option.map(((moddedDuration, songs)) =>
       (
         moddedDuration > songs[0]##duration ? songs[1] : songs[0],
         moddedDuration
         -. (moddedDuration > songs[0]##duration ? songs[0]##duration : 0.0),
       )
     )
  |. Belt.Option.map(((song, moddedDuration)) =>
       (song, getSongPath(song), moddedDuration)
     )
  |> Belt.Option.getExn;
