let yearLabelStyle = isFocused =>
  ReactDOMRe.Style.make(
    ~position="relative",
    ~top="15px",
    ~left="50%",
    ~color="#555",
    ~fontSize="22px",
    ~opacity=isFocused ? "1" : "0",
    (),
  );

let render =
    (
      ~width,
      ~scrollLeftByYear,
      ~onClickPhoto,
      ~onHoverPhoto,
      ~onScrollLeft,
      ~setRef,
      ~focusedRowIndex,
      ~refByYears,
      props,
    ) => {
  let rowIndex = props##rowIndex;
  let year = ImageMetadata.years[rowIndex];
  let isFocused = focusedRowIndex == rowIndex;
  <div key=props##key style=props##style>
    <span style=(isFocused |> yearLabelStyle)>
      (ReasonReact.string(ImageMetadata.years[props##rowIndex]))
    </span>
    <ScrollButton
      className="fa fa-chevron-left"
      isFocused
      year
      refByYears
      scrollLeftByYear
      scrollDelta=(width *. (-1.0))
      style=(ReactDOMRe.Style.make(~left="10px", ()))
    />
    <ScrollButton
      className="fa fa-chevron-right"
      isFocused
      year
      refByYears
      scrollLeftByYear
      scrollDelta=width
      style=(ReactDOMRe.Style.make(~right="15px", ()))
    />
    <ReactVirtualized.Grid
      cellRenderer=(
        Photo.renderer(
          ~rowIndex,
          ~focusedRowIndex,
          ~onClick=columnIndex => onClickPhoto((year, columnIndex)),
          ~onHover=(_) => onHoverPhoto(rowIndex),
        )
      )
      className=("year-" ++ props##key)
      columnCount=(
        ImageMetadata.countByYear
        |> Belt.Map.String.getWithDefault(_, year, 0)
      )
      columnWidth=(Photo.getColumnWidth(rowIndex))
      height=Constants.rowHeight
      onScroll=(scrollEvent => onScrollLeft(year, scrollEvent##scrollLeft))
      ref=(setRef(year))
      rowHeight=Constants.rowHeight
      rowCount=1
      scrollLeft=(Belt.Map.String.getWithDefault(scrollLeftByYear, year, 0.0))
      width
    />
  </div>;
};
