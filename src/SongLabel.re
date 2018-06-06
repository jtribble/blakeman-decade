let component = ReasonReact.statelessComponent("SongLabel");

let make = (~artist, ~song, _children) => {
  ...component,
  render: (_) =>
    <div
      style=(
        ReactDOMRe.Style.make(
          ~fontSize="16px",
          ~position="absolute",
          ~padding="20px",
          ~bottom="20px",
          ~right="20px",
          ~backgroundColor="#333",
          ~color="#fff",
          ~display="flex",
          ~alignItems="center",
          ~zIndex="1",
          (),
        )
      )>
      (ReasonReact.stringToElement("\"" ++ song ++ "\"" ++ " by " ++ artist))
    </div>,
};
