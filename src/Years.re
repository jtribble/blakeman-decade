module Window = {
  [@bs.val] [@bs.scope "window"] external innerHeight : float = "";
  [@bs.val] [@bs.scope "window"] external innerWidth : float = "";
  [@bs.val] [@bs.scope "window"] external pageYOffset : float = "";
  [@bs.val] [@bs.scope "window"]
  external addEventListener : (string, unit => unit, bool) => unit = "";
  [@bs.val] [@bs.scope "window"]
  external removeEventListener : (string, unit => unit, bool) => unit = "";
};

let years = [|"2008", "2009", "2010", "2011", "2012"|];

let photoRenderer = props =>
  <div key=props##key style=props##style>
    (ReasonReact.stringToElement(props##columnIndex))
  </div>;

let yearRenderer = (windowWidth, scrollLeftByYear, onScrollLeft, props) =>
  <div key=props##key style=props##style>
    <ReactVirtualized.Grid
      cellRenderer=photoRenderer
      className=("year-" ++ props##key)
      columnCount=50
      columnWidth=750
      height=500.0
      onScroll=(
        scrollEvent =>
          onScrollLeft(years[props##rowIndex], scrollEvent##scrollLeft)
      )
      rowHeight=500
      rowCount=1
      scrollLeft=(
        Belt.Map.String.getWithDefault(
          scrollLeftByYear,
          years[props##rowIndex],
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
  | SetWindowSize
  | SetScrollLeft(string, float);

let component = ReasonReact.reducerComponent("Years");

let make = _children => {
  ...component,
  initialState: () => {
    focusedRowIndex: 0,
    scrollLeftByYear:
      Belt.Map.String.fromArray([|
        ("2008", 0.0),
        ("2009", 0.0),
        ("2010", 0.0),
        ("2011", 0.0),
        ("2012", 0.0),
      |]),
    scrollY: Window.pageYOffset,
    windowHeight: Window.innerHeight,
    windowWidth: Window.innerWidth,
  },
  reducer: (action, state) =>
    switch (action) {
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
          self.state.windowWidth, self.state.scrollLeftByYear, (year, scroll) =>
          self.send(SetScrollLeft(year, scroll))
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
