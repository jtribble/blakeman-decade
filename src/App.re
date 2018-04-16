type page =
  | Years
  | Intro;

type action =
  | ShowYears
  | ShowIntro;

type state = {nowShowing: page};

let component = ReasonReact.reducerComponent("App");

let getPage = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | ["years"] => Years
  | _ => Intro
  };

let make = _children => {
  ...component,
  initialState: () => {
    nowShowing: ReasonReact.Router.dangerouslyGetInitialUrl() |> getPage,
  },
  reducer: (action, _) =>
    switch (action) {
    | ShowYears => ReasonReact.Update({nowShowing: Years})
    | ShowIntro => ReasonReact.Update({nowShowing: Intro})
    },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          switch (url |> getPage) {
          | Years => self.send(ShowYears)
          | Intro => self.send(ShowIntro)
          }
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  render: ({state: {nowShowing}}) =>
    switch (nowShowing) {
    | Years => <div> (ReasonReact.stringToElement("Years!")) </div>
    | Intro => <div> <Intro /> </div>
    },
};
