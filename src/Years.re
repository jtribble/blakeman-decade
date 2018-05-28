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

let imageStyle = (width, isFocused) =>
  ReactDOMRe.Style.make(
    ~height=(Constants.imageHeight |> string_of_float) ++ "px",
    ~opacity=isFocused ? "1" : "0.8",
    ~width=(width |> string_of_float) ++ "px",
    (),
  );

let photoRenderer = (~rowIndex, ~focusedRowIndex, ~onHover, props) => {
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
          style=(imageStyle(imageWidth, rowIndex == focusedRowIndex))
        />
      </div>
    </div>
  </div>;
};

let yearLabelStyle = isFocused =>
  ReactDOMRe.Style.make(
    ~position="relative",
    ~top="15px",
    ~left="30px",
    ~color="#555",
    ~fontSize="22px",
    ~opacity=isFocused ? "1" : "0",
    (),
  );

let yearRenderer =
    (
      ~width,
      ~scrollLeftByYear,
      ~onHoverPhoto,
      ~onScrollLeft,
      ~setRef,
      ~focusedRowIndex,
      props,
    ) => {
  let rowIndex = props##rowIndex;
  let year = ImageMetadata.years[rowIndex];
  <div key=props##key style=props##style>
    <span style=(focusedRowIndex == rowIndex |> yearLabelStyle)>
      (ReasonReact.stringToElement(ImageMetadata.years[props##rowIndex]))
    </span>
    <a
      style=(
        ReactDOMRe.Style.make(
          ~left="0",
          ~position="absolute",
          ~color="red",
          ~zIndex="1",
          ~top="50%",
          (),
        )
      )>
      (ReasonReact.stringToElement("Scroll left"))
    </a>
    <a
      style=(
        ReactDOMRe.Style.make(
          ~right="0",
          ~position="absolute",
          ~color="red",
          ~zIndex="1",
          ~top="50%",
          (),
        )
      )>
      (ReasonReact.stringToElement("Scroll right"))
    </a>
    <ReactVirtualized.Grid
      cellRenderer=(
        photoRenderer(~rowIndex, ~focusedRowIndex, ~onHover=(_) =>
          onHoverPhoto(rowIndex)
        )
      )
      className=("year-" ++ props##key)
      columnCount=(
        ImageMetadata.countByYear
        |> Belt.Map.String.getWithDefault(_, year, 0)
      )
      columnWidth=(getColumnWidth(rowIndex))
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
          ~onHoverPhoto=index => self.send(FocusRow(index)),
          ~onScrollLeft=
            (year, scroll) => self.send(SetScrollLeft(year, scroll)),
          ~setRef=year => self.handle(setYearRef(year)),
          ~focusedRowIndex=self.state.focusedRowIndex,
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
