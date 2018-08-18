let component = ReasonReact.statelessComponent("SongLabel");

let make = (~artist, ~isMuted, ~mute, ~song, ~unmute, _children) => {
  ...component,
  render: _ =>
    <div
      style=(
        ReactDOMRe.Style.make(
          ~fontSize="16px",
          ~position="absolute",
          ~padding="20px",
          ~bottom="20px",
          ~right="20px",
          ~backgroundColor="#fff",
          ~color="#333",
          ~display="flex",
          ~alignItems="center",
          ~zIndex="3",
          (),
        )
      )>
      <div style=(ReactDOMRe.Style.make(~marginRight="10px", ()))>
        (ReasonReact.string("\"" ++ song ++ "\"" ++ " by " ++ artist))
      </div>
      (
        isMuted ?
          <a
            onClick=(_ => unmute())
            style=(
              ReactDOMRe.Style.make(
                ~cursor="pointer",
                ~textDecoration="underline",
                ~color="palevioletred",
                (),
              )
            )>
            (ReasonReact.string("Unmute"))
          </a> :
          <a
            onClick=(_ => mute())
            style=(
              ReactDOMRe.Style.make(
                ~cursor="pointer",
                ~textDecoration="underline",
                ~color="palevioletred",
                (),
              )
            )>
            (ReasonReact.string("Mute"))
          </a>
      )
    </div>,
};
