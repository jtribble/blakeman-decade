[@bs.val] [@bs.module]
external imageMetadata :
  Js.Dict.t(
    Js.Dict.t(
      {
        .
        "height": int,
        "width": int,
      },
    ),
  ) =
  "./constants/image-metadata.json";

let countByYear =
  imageMetadata
  |> Js.Dict.keys
  |> Array.map(key =>
       (
         key,
         Js.Dict.get(imageMetadata, key)
         |> Js.Option.getExn
         |> Js.Dict.keys
         |> Array.length,
       )
     )
  |> Belt.Map.String.fromArray;

let years = imageMetadata |> Js.Dict.keys;

let getImagePaths = (size, year) =>
  Belt.Map.String.getWithDefault(countByYear, year, 0)
  |> Belt.Array.makeBy(_, i => i + 1)
  |> Belt.Array.map(_, i =>
       "/assets/"
       ++ year
       ++ "/"
       ++ size
       ++ "/"
       ++ year
       ++ "-"
       ++ string_of_int(i)
       ++ ".jpg"
     );

let getSmallImagePaths = getImagePaths("sm");
