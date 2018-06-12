let getImageWidth = (rowIndex, columnIndex) =>
  ImageMetadata.getImageDimensions(
    ImageMetadata.years[rowIndex],
    columnIndex + 1 |> string_of_int,
  )
  |> (
    dimens => (dimens##height |> float_of_int, dimens##width |> float_of_int)
  )
  |> (((h, w)) => w *. Constants.imageHeight /. h);

let getColumnWidth = (rowIndex, columnInfo) =>
  getImageWidth(rowIndex, columnInfo##index)
  +. 2.0
  *. (Constants.photoPadding +. Constants.imagePadding);

let photoContainerStyle =
  ReactDOMRe.Style.make(
    ~height="100%",
    ~padding=string_of_int(Constants.photoPadding |> int_of_float) ++ "px",
    ~position="relative",
    ~width="auto",
    (),
  );

let photoStyle = width =>
  ReactDOMRe.Style.make(
    ~alignItems="center",
    ~backgroundColor="white",
    ~boxShadow="0px 3px 15px rgba(0,0,0,0.2)",
    ~cursor="pointer",
    ~display="flex",
    ~justifyContent="center",
    ~height=(Constants.photoHeight |> string_of_float) ++ "px",
    ~margin=(Constants.imagePadding |> string_of_float) ++ "px",
    ~width=(width |> string_of_float) ++ "px",
    (),
  );

let imageStyle = (width, isFocused) =>
  ReactDOMRe.Style.make(
    ~height=(Constants.imageHeight |> string_of_float) ++ "px",
    ~opacity=isFocused ? "1" : "0.8",
    ~width=(width |> string_of_float) ++ "px",
    (),
  );

let renderer = (~rowIndex, ~focusedRowIndex, ~onClick, ~onHover, props) => {
  let imageWidth = getImageWidth(rowIndex, props##columnIndex);
  <div key=props##key style=props##style>
    <div onMouseOver=onHover style=photoContainerStyle>
      <div
        onClick=((_) => onClick(props##columnIndex))
        style=(photoStyle(imageWidth +. 2.0 *. Constants.imagePadding))>
        <img
          src=(
            ImageMetadata.years[rowIndex]
            |> ImageMetadata.getSmallImagePaths
            |> Belt.Array.getExn(_, props##columnIndex)
          )
          style=(imageStyle(imageWidth, rowIndex == focusedRowIndex))
        />
      </div>
    </div>
  </div>;
};
