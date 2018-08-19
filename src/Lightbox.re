type state = {lightboxRef: ref(option(Dom.element))};

type action =
  | Focus;

let component = ReasonReact.reducerComponent("Lightbox");

let setLightboxRef = (r, {ReasonReact.state}) =>
  state.lightboxRef := Js.Nullable.toOption(r);

let make =
    (
      ~artist,
      ~close,
      ~height,
      ~isMuted,
      ~mute,
      ~path,
      ~prevPhoto,
      ~nextPhoto,
      ~song,
      ~unmute,
      ~width,
      ~year,
      _children,
    ) => {
  ...component,
  initialState: () => {lightboxRef: ref(None)},
  reducer: action =>
    switch (action) {
    | Focus => (
        _ =>
          ReasonReact.SideEffects(
            ({state}) =>
              switch (state.lightboxRef^) {
              | Some(field) =>
                let node = ReactDOMRe.domElementToObj(field);
                ignore(node##focus());
              | _ => ()
              },
          )
      )
    },
  didMount: self => self.send(Focus),
  render: ({handle}) =>
    <div
      ref=(handle(setLightboxRef))
      style=(ReactDOMRe.Style.make(~position="absolute", ~top="0", ()))
      tabIndex=(-1)>
      <div
        style=(
          ReactDOMRe.Style.make(
            ~backgroundColor="#000",
            ~opacity="1",
            ~position="absolute",
            ~width=Window.floatToJsString(width) ++ "px",
            ~height=Window.floatToJsString(height) ++ "px",
            ~zIndex="1",
            (),
          )
        )
      />
      <SongLabel artist song isMuted mute unmute />
      <div
        style=(
          ReactDOMRe.Style.make(
            ~color="white",
            ~position="relative",
            ~fontSize="26px",
            ~textAlign="left",
            ~paddingLeft="20px",
            ~top="10px",
            ~zIndex="1",
            (),
          )
        )>
        (year |. ReasonReact.string)
      </div>
      <i
        className="fa fa-chevron-left"
        onClick=prevPhoto
        style=(
          ReactDOMRe.Style.make(
            ~color="#F2F2F2",
            ~cursor="pointer",
            ~position="absolute",
            ~left="10px",
            ~zIndex="3",
            ~top="50%",
            ~fontSize="50px",
            (),
          )
        )
      />
      <i
        className="fa fa-chevron-right"
        onClick=nextPhoto
        style=(
          ReactDOMRe.Style.make(
            ~color="#F2F2F2",
            ~cursor="pointer",
            ~fontSize="50px",
            ~position="absolute",
            ~right="50px",
            ~zIndex="3",
            ~top="50%",
            (),
          )
        )
      />
      <div
        onClick=close
        style=(
          ReactDOMRe.Style.make(
            ~display="flex",
            ~alignItems="center",
            ~justifyContent="center",
            ~position="relative",
            ~zIndex="2",
            ~width=Window.floatToJsString(width) ++ "px",
            ~height=Window.floatToJsString(height) ++ "px",
            (),
          )
        )>
        <img
          onClick=nextPhoto
          style=(
            ReactDOMRe.Style.make(
              ~maxWidth=Window.floatToJsString(width -. 200.01) ++ "px",
              ~maxHeight=Window.floatToJsString(height -. 50.01) ++ "px",
              (),
            )
          )
          src=path
        />
      </div>
    </div>,
};
