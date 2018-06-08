let component = ReasonReact.statelessComponent("Lightbox");

let make = (~height, ~path, ~width, _children) => {
  ...component,
  render: (_) =>
    <div
      style=(
        ReactDOMRe.Style.make(
          ~position="absolute",
          ~width=string_of_float(width) ++ "px",
          ~height=string_of_float(height) ++ "px",
          ~top="0",
          (),
        )
      )>
      <div style=(ReactDOMRe.Style.make())>
        <img style=(ReactDOMRe.Style.make(~width="100%", ())) src=path />
      </div>
    </div>,
};
