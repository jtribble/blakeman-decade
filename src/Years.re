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
    ~left="50%",
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
      ~refByYears,
      props,
    ) => {
  let rowIndex = props##rowIndex;
  let year = ImageMetadata.years[rowIndex];
  let isFocused = focusedRowIndex == rowIndex;
  <div key=props##key style=props##style>
    <span style=(isFocused |> yearLabelStyle)>
      (ReasonReact.stringToElement(ImageMetadata.years[props##rowIndex]))
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

let ensureFocusedRowIsScrolled =
    (~yearsRef, ~scrollLeftByYear, ~focusedRowIndex) =>
  ReactVirtualized.Grid.scrollToCell(
    yearsRef^ |> Belt.Option.getExn,
    {
      "columnIndex":
        Belt.Map.String.getExn(
          scrollLeftByYear,
          ImageMetadata.years[focusedRowIndex],
        )
        |> (scrollLeft => scrollLeft /. 450.0)
        |> int_of_float,
      "rowIndex": focusedRowIndex,
    },
  );

type state = {
  focusedRowIndex: int,
  refByYears: ref(Belt.Map.String.t(option(ReasonReact.reactRef))),
  yearsRef: ref(option(ReasonReact.reactRef)),
  scrollLeftByYear: Belt.Map.String.t(float),
  scrollY: float,
  windowHeight: float,
  windowWidth: float,
};

let setYearsRef = (theRef, {ReasonReact.state}) =>
  state.yearsRef := Js.Nullable.toOption(theRef);

let setYearRef = (year, theRef, {ReasonReact.state}) =>
  state.refByYears :=
    state.refByYears^
    |. Belt.Map.String.set(year, theRef |> Js.Nullable.toOption);

type action =
  | FocusRow(int)
  | KeyDown(int)
  | SetWindowSize
  | SetScrollLeft(string, float);

let component = ReasonReact.reducerComponent("Years");

let make = _children => {
  ...component,
  initialState: () => {
    focusedRowIndex: 0,
    refByYears:
      ImageMetadata.years
      |> Array.map(year => (year, None))
      |> Belt.Map.String.fromArray
      |> ref,
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
    | KeyDown(keycode) =>
      switch (keycode) {
      | 40 =>
        /* Down */
        ReasonReact.UpdateWithSideEffects(
          {
            ...state,
            focusedRowIndex:
              Js.Math.min_int(
                ImageMetadata.years |> Array.length |> (x => x - 1),
                state.focusedRowIndex + 1,
              ),
          },
          (
            self =>
              ensureFocusedRowIsScrolled(
                ~yearsRef=self.state.yearsRef,
                ~scrollLeftByYear=self.state.scrollLeftByYear,
                ~focusedRowIndex=self.state.focusedRowIndex,
              )
          ),
        )
      | 38 =>
        /* Up */
        ReasonReact.UpdateWithSideEffects(
          {
            ...state,
            focusedRowIndex: Js.Math.max_int(0, state.focusedRowIndex - 1),
          },
          (
            self =>
              ensureFocusedRowIsScrolled(
                ~yearsRef=self.state.yearsRef,
                ~scrollLeftByYear=self.state.scrollLeftByYear,
                ~focusedRowIndex=self.state.focusedRowIndex,
              )
          ),
        )
      | 37 =>
        /* Left */
        ReasonReact.SideEffects(
          (
            self =>
              ScrollButton.scrollRow(
                ~refByYears=self.state.refByYears,
                ~year=ImageMetadata.years[self.state.focusedRowIndex],
                ~scrollLeftByYear=self.state.scrollLeftByYear,
                ~scrollDelta=self.state.windowWidth *. (-0.7),
              )
          ),
        )
      | 39 =>
        /* Left */
        ReasonReact.SideEffects(
          (
            self =>
              ScrollButton.scrollRow(
                ~refByYears=self.state.refByYears,
                ~year=ImageMetadata.years[self.state.focusedRowIndex],
                ~scrollLeftByYear=self.state.scrollLeftByYear,
                ~scrollDelta=self.state.windowWidth *. 0.7,
              )
          ),
        )
      | _ => ReasonReact.NoUpdate
      }
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
    Sub(
      () =>
        Body.addEventListener("keydown", event =>
          self.send(KeyDown(event##keyCode))
        ),
      () =>
        Body.removeEventListener("keydown", event =>
          self.send(KeyDown(event##keyCode))
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
          ~refByYears=self.state.refByYears,
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
