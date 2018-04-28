let photoContainerStyle =
  ReactDOMRe.Style.make(
    ~boxShadow="0px 3px 15px rgba(0, 0, 0, 0.2)",
    ~height="100%",
    ~position="relative",
    ~width="auto",
    (),
  );

let photoStyle = ReactDOMRe.Style.make(~height="100%", ());

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

let yearRenderer =
    (windowWidth, scrollLeftByYear, onHover, onScrollLeft, props) =>
  <div key=props##key style=props##style>
    <ReactVirtualized.Grid
      cellRenderer=(
        photoRenderer(props##rowIndex, (_) => onHover(props##rowIndex))
      )
      className=("year-" ++ props##key)
      columnCount=50
      columnWidth=750
      height=500.0
      onScroll=(
        scrollEvent =>
          onScrollLeft(
            ImageMetadata.years[props##rowIndex],
            scrollEvent##scrollLeft,
          )
      )
      rowHeight=500
      rowCount=1
      scrollLeft=(
        Belt.Map.String.getWithDefault(
          scrollLeftByYear,
          ImageMetadata.years[props##rowIndex],
          0.0,
        )
      )
      width=windowWidth
    />
  </div>;

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
      rowCount=5
      rowHeight=500
      width=self.state.windowWidth
    />,
};
