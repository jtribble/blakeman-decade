module Window = {
  [@bs.val] [@bs.scope "window"] external innerHeight : float = "";
  [@bs.val] [@bs.scope "window"] external innerWidth : float = "";
  [@bs.val] [@bs.scope "window"] external pageYOffset : float = "";
  [@bs.val] [@bs.scope "window"]
  external addEventListener : (string, unit => unit, bool) => unit = "";
  [@bs.val] [@bs.scope "window"]
  external removeEventListener : (string, unit => unit, bool) => unit = "";
};

/* State declaration */
type state = {
  focusedRowIndex: int,
  scrollLeftByYear: {
    .
    "2008": float,
    "2009": float,
    "2010": float,
    "2011": float,
    "2012": float,
  },
  scrollY: float,
  windowHeight: float,
  windowWidth: float,
};

type action =
  | SetWindowSize;

let component = ReasonReact.reducerComponent("Years");

let make = _children => {
  ...component,
  initialState: () => {
    focusedRowIndex: 0,
    scrollLeftByYear: {
      "2008": 0.0,
      "2009": 0.0,
      "2010": 0.0,
      "2011": 0.0,
      "2012": 0.0,
    },
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
  render: _self => <div />,
};
