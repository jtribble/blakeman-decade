let component = ReasonReact.statelessComponent("SongLabel");

let make = (~artist, ~song, _children) => {
  ...component,
  render: (_) =>
    <div style=(ReactDOMRe.Style.make(~marginRight="10px", ()))>
      (ReasonReact.string("\"" ++ song ++ "\"" ++ " by " ++ artist))
    </div>,
};
