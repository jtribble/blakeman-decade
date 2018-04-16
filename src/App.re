type page =
  | Years
  | Intro;

type action =
  | ShowYears
  | ShowIntro;

type state = {nowShowing: page};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {nowShowing: Intro},
  reducer: (action, _) =>
    switch (action) {
    | ShowYears => ReasonReact.Update({nowShowing: Years})
    | ShowIntro => ReasonReact.Update({nowShowing: Intro})
    },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          switch (url.path) {
          | ["years"] => self.send(ShowYears)
          | _ => self.send(ShowIntro)
          }
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  render: ({state: {nowShowing}}) =>
    switch (nowShowing) {
    | Years => <div> (ReasonReact.stringToElement("Years!")) </div>
    | Intro => <div> (ReasonReact.stringToElement("Intro!")) </div>
    },
};
