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
             |> Js.Math.max_float(0.0),
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
      className
      onClick=(
        (_) => scrollRow(~refByYears, ~year, ~scrollLeftByYear, ~scrollDelta)
      )
      style=(
        ReactDOMRe.Style.combine(
          style,
          ReactDOMRe.Style.make(
            ~fontSize="30px",
            ~position="absolute",
            ~color="#555",
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
