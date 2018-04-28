module Constants = {
  let rowHeight = 350.0;
  let columnWidth = 600.0;
  let imagePadding = 30.0;
  let imageHeight = rowHeight -. 2.0 *. imagePadding;
};

let photoContainerStyle =
  ReactDOMRe.Style.make(
    ~height="100%",
    ~padding="30px",
    ~position="relative",
    ~width="auto",
    (),
  );

let photoStyle =
  ReactDOMRe.Style.make(
    ~height=(Constants.imageHeight |> string_of_float) ++ "px",
    (),
  );

let photoRenderer = (rowIndex, onHover, props) =>
  <div key=props##key style=props##style>
    <div onMouseOver=onHover style=photoContainerStyle>
      <img
        src=(
          ImageMetadata.years[rowIndex]
          |> ImageMetadata.getSmallImagePaths
          |> Belt.Array.getExn(_, props##columnIndex)
        )
        style=photoStyle
      />
    </div>
  </div>;

let getColumnWidth = (rowIndex, columnInfo) =>
  ImageMetadata.getImageDimensions(
    ImageMetadata.years[rowIndex],
    columnInfo##index + 1 |> string_of_int,
  )
  |> (
    dimens => (dimens##height |> float_of_int, dimens##width |> float_of_int)
  )
  |> (
    ((h, w)) =>
      w *. Constants.imageHeight /. h +. 2.0 *. Constants.imagePadding
  );

let yearRenderer =
    (windowWidth, scrollLeftByYear, onHover, onScrollLeft, props) => {
  let year = ImageMetadata.years[props##rowIndex];
  <div key=props##key style=props##style>
    <ReactVirtualized.Grid
      cellRenderer=(
        photoRenderer(props##rowIndex, (_) => onHover(props##rowIndex))
      )
      className=("year-" ++ props##key)
      columnCount=(
        ImageMetadata.countByYear
        |> Belt.Map.String.getWithDefault(_, year, 0)
      )
      columnWidth=(getColumnWidth(props##rowIndex))
      height=Constants.rowHeight
      onScroll=(scrollEvent => onScrollLeft(year, scrollEvent##scrollLeft))
      rowHeight=Constants.rowHeight
      rowCount=1
      scrollLeft=(Belt.Map.String.getWithDefault(scrollLeftByYear, year, 0.0))
      width=windowWidth
    />
  </div>;
};

type state = {
  focusedRowIndex: int,
  scrollLeftByYear: Belt.Map.String.t(float),
  scrollY: float,
  windowHeight: float,
  windowWidth: float,
};

type action =
  | FocusRow(int)
  | SetWindowSize
  | SetScrollLeft(string, float);

let component = ReasonReact.reducerComponent("Years");

let make = _children => {
  ...component,
  initialState: () => {
    focusedRowIndex: 0,
    scrollLeftByYear:
      ImageMetadata.years
      |> Array.map(year => (year, 0.0))
      |> Belt.Map.String.fromArray,
    scrollY: Window.pageYOffset,
    windowHeight: Window.innerHeight,
    windowWidth: Window.innerWidth,
  },
  reducer: (action, state) =>
    switch (action) {
    | FocusRow(index) =>
      ReasonReact.Update({...state, focusedRowIndex: index})
    | SetWindowSize =>
      ReasonReact.Update({
        ...state,
        windowHeight: Window.innerHeight,
        windowWidth: Window.innerWidth,
      })
    | SetScrollLeft(year, scroll) =>
      ReasonReact.Update({
        ...state,
        scrollLeftByYear:
          Belt.Map.String.set(state.scrollLeftByYear, year, scroll),
      })
    },
  subscriptions: self => [
    Sub(
      () =>
        Window.addEventListener(
          "resize",
          () => self.send(SetWindowSize),
          true,
        ),
      () =>
        Window.removeEventListener(
          "resize",
          () => self.send(SetWindowSize),
          true,
        ),
    ),
  ],
  render: self =>
    <ReactVirtualized.Grid
      cellRenderer=(
        yearRenderer(
          self.state.windowWidth,
          self.state.scrollLeftByYear,
          index => self.send(FocusRow(index)),
          (year, scroll) => self.send(SetScrollLeft(year, scroll)),
        )
      )
      className="years"
      columnCount=1
      columnWidth=self.state.windowWidth
      height=self.state.windowHeight
      rowCount=(ImageMetadata.years |> Array.length)
      rowHeight=Constants.rowHeight
      width=self.state.windowWidth
    />,
};
