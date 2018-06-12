type state = {lightboxRef: ref(option(Dom.element))};

type action =
  | Focus;

let component = ReasonReact.reducerComponent("Lightbox");

let setLightboxRef = (r, {ReasonReact.state}) =>
  state.lightboxRef := Js.Nullable.toOption(r);

let make = (~close, ~height, ~path, ~prevPhoto, ~nextPhoto, ~width, _children) => {
  ...component,
  initialState: () => {lightboxRef: ref(None)},
  reducer: action =>
    switch (action) {
    | Focus => (
        (_) =>
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
            ~opacity="0.7",
            ~position="absolute",
            ~width=string_of_float(width) ++ "px",
            ~height=string_of_float(height) ++ "px",
            ~zIndex="1",
            (),
          )
        )
      />
      <i
        className="fa fa-chevron-left"
        onClick=prevPhoto
        style=(
          ReactDOMRe.Style.make(
            ~color="white",
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
            ~color="white",
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
            ~width=string_of_float(width) ++ "px",
            ~height=string_of_float(height) ++ "px",
            (),
          )
        )>
        <img
          onClick=nextPhoto
          style=(
            ReactDOMRe.Style.make(
              ~maxWidth=string_of_float(width -. 200.01) ++ "px",
              ~maxHeight=string_of_float(height -. 50.01) ++ "px",
              (),
            )
          )
          src=path
        />
      </div>
    </div>,
};
