let getRowWidth = year =>
  ImageMetadata.countByYear
  |. Belt.Map.String.getExn(year)
  |. Belt.Array.makeBy(columnIndex =>
       Photo.getColumnWidth(ImageMetadata.yearToRowIndex(year), columnIndex)
     )
  |. Belt.Array.reduce(0.0, (a, b) => a +. b);

let scrollRow = (~refByYears, ~year, ~scrollLeftByYear, ~scrollDelta) =>
  refByYears^
  |> Belt.Map.String.getExn(_, year)
  |. Belt.Option.map(
       ReactVirtualized.Grid.scrollToPosition(
         _,
         {
           "scrollLeft":
             Belt.Map.String.getExn(scrollLeftByYear, year)
             |> (left => left +. scrollDelta)
             |> Js.Math.max_float(0.0)
             |> Js.Math.min_float(getRowWidth(year) *. 0.95),
           "scrollTop": 0.0,
         },
       ),
     )
  |> ignore;

let component = ReasonReact.statelessComponent("Greeting");

let make =
    (
      ~className,
      ~isFocused,
      ~year,
      ~refByYears,
      ~scrollLeftByYear,
      ~scrollDelta,
      ~style,
      _children,
    ) => {
  ...component,
  render: (_) =>
    <i
      className=(className ++ " chevron-button")
      onClick=(
        (_) => scrollRow(~refByYears, ~year, ~scrollLeftByYear, ~scrollDelta)
      )
      style=(
        ReactDOMRe.Style.combine(
          style,
          ReactDOMRe.Style.make(
            ~color="black",
            ~fontSize="25px",
            ~padding="5px",
            ~position="absolute",
            ~cursor="pointer",
            ~zIndex="1",
            ~top="50%",
            ~opacity=isFocused ? "1" : "0",
            (),
          ),
        )
      )
    />,
};
