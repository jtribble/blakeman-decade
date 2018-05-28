module Constants = {
  let rowHeight = 350.0;
  let columnWidth = 600.0;
  let photoPadding = 30.0;
  let photoHeight = rowHeight -. 2.0 *. photoPadding;
  let imagePadding = 20.0;
  let imageHeight = photoHeight -. 2.0 *. imagePadding;
};

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
    ~display="flex",
    ~justifyContent="center",
    ~height=(Constants.photoHeight |> string_of_float) ++ "px",
    ~margin=(Constants.imagePadding |> string_of_float) ++ "px",
    ~width=(width |> string_of_float) ++ "px",
    (),
  );

let imageStyle = width =>
  ReactDOMRe.Style.make(
    ~height=(Constants.imageHeight |> string_of_float) ++ "px",
    ~width=(width |> string_of_float) ++ "px",
    (),
  );

let photoRenderer = (~rowIndex, ~onHover, props) => {
  let imageWidth = getImageWidth(rowIndex, props##columnIndex);
  <div key=props##key style=props##style>
    <div onMouseOver=onHover style=photoContainerStyle>
      <div style=(photoStyle(imageWidth +. 2.0 *. Constants.imagePadding))>
        <img
          src=(
            ImageMetadata.years[rowIndex]
            |> ImageMetadata.getSmallImagePaths
            |> Belt.Array.getExn(_, props##columnIndex)
          )
          style=(imageWidth |> imageStyle)
        />
      </div>
    </div>
  </div>;
};

let yearRenderer =
    (~width, ~scrollLeftByYear, ~onHoverRow, ~onScrollLeft, ~setRef, props) => {
  let year = ImageMetadata.years[props##rowIndex];
  <div key=props##key style=props##style>
    <ReactVirtualized.Grid
      cellRenderer=(
        photoRenderer(~rowIndex=props##rowIndex, ~onHover=(_) =>
          onHoverRow(props##rowIndex)
        )
      )
      className=("year-" ++ props##key)
      columnCount=(
        ImageMetadata.countByYear
        |> Belt.Map.String.getWithDefault(_, year, 0)
      )
      columnWidth=(getColumnWidth(props##rowIndex))
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

type state = {
  focusedRowIndex: int,
  refByYears: Belt.Map.String.t(ref(option(ReasonReact.reactRef))),
  yearsRef: ref(option(ReasonReact.reactRef)),
  scrollLeftByYear: Belt.Map.String.t(float),
  scrollY: float,
  windowHeight: float,
  windowWidth: float,
};

let setYearsRef = (theRef, {ReasonReact.state}) =>
  state.yearsRef := Js.Nullable.toOption(theRef);

let setYearRef = (year, theRef, {ReasonReact.state}) =>
  Belt.Map.String.set(
    state.refByYears,
    year,
    ref(Js.Nullable.toOption(theRef)),
  )
  |> ignore;

type action =
  | FocusRow(int)
  | SetWindowSize
  | SetScrollLeft(string, float);

let component = ReasonReact.reducerComponent("Years");

let make = _children => {
  ...component,
  initialState: () => {
    focusedRowIndex: 0,
    refByYears:
      ImageMetadata.years
      |> Array.map(year => (year, ref(None)))
      |> Belt.Map.String.fromArray,
    scrollLeftByYear:
      ImageMetadata.years
      |> Array.map(year => (year, 0.0))
      |> Belt.Map.String.fromArray,
    scrollY: Window.pageYOffset,
    windowHeight: Window.innerHeight,
    windowWidth: Window.innerWidth,
    yearsRef: ref(None),
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
          ~width=self.state.windowWidth,
          ~scrollLeftByYear=self.state.scrollLeftByYear,
          ~onHoverRow=index => self.send(FocusRow(index)),
          ~onScrollLeft=
            (year, scroll) => self.send(SetScrollLeft(year, scroll)),
          ~setRef=year => self.handle(setYearRef(year)),
        )
      )
      className="years"
      columnCount=1
      columnWidth=self.state.windowWidth
      height=self.state.windowHeight
      ref=(self.handle(setYearsRef))
      rowCount=(ImageMetadata.years |> Array.length)
      rowHeight=Constants.rowHeight
      scrollToAlignment="center"
      width=self.state.windowWidth
    />,
};
