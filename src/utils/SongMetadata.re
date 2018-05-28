[@bs.val] [@bs.module]
external songMetadata :
  Js.Dict.t(
    Js.Array.t(
      {
        .
        "artist": string,
        "duration": float,
        "song": string,
      },
    ),
  ) =
  "../constants/song-metadata.json";

let getSongPath = song => "/songs/" ++ song##song ++ ".mp3";

let getSong = (~year, ~duration) =>
  songMetadata
  |. Js.Dict.get(year)
  |. Belt.Option.map(songs =>
       mod_float(duration, songs[0]##duration +. songs[1]##duration)
       > songs[0]##duration ?
         songs[0] : songs[1]
     )
  |. Belt.Option.map(getSongPath)
  |> Belt.Option.getExn;
