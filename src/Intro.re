let component = ReasonReact.statelessComponent("Intro");

let letterStyle =
  ReactDOMRe.Style.make(
    ~flexDirection="column",
    ~margin="35px auto",
    ~maxWidth="600",
    ~minWidth="300px",
    ~display="flex",
    ~justifyContent="center",
    ~alignItems="center",
    ~width="60%",
    (),
  );

let letterSignatureStyle =
  ReactDOMRe.Style.make(
    ~fontSize="1.25em",
    ~fontWeight="bold",
    ~textAlign="right",
    (),
  );

let linkStyle =
  ReactDOMRe.Style.make(
    ~textAlign="center",
    ~width="100%",
    ~display="block",
    ~color="palevioletred",
    (),
  );

let letterTextStyle = ReactDOMRe.Style.make(~textIndent="3em", ());

let letterTitleStyle =
  ReactDOMRe.Style.make(~marginTop="0", ~textAlign="center", ());

let str = ReasonReact.string;

let make = _children => {
  ...component,
  render: _self =>
    <div style=letterStyle>
      <h1 style=letterTitleStyle> (str("Together Through Life")) </h1>
      <p style=letterTextStyle>
        (
          str(
            "Nullam finibus lacinia dolor, nec auctor dui tincidunt ornare. Phasellus in urna eget eros ornare mattis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Donec interdum mattis scelerisque. Integer massa nisl, fringilla vel magna eu, eleifend dignissim erat. Vivamus non ligula non ipsum gravida suscipit. Proin eu mattis odio. Mauris ut urna id nunc elementum commodo. Suspendisse in turpis efficitur, mollis sem non, eleifend lorem. Aenean turpis velit, consequat a turpis a, imperdiet mattis",
          )
        )
      </p>
      <p style=letterTextStyle>
        (
          str(
            "Nullam dapibus quam ac massa varius eleifend. Integer faucibus non mi nec imperdiet. Etiam aliquet elementum elit, sed fermentum elit feugiat eu. Etiam eu ex quis neque varius condimentum quis quis arcu. Sed ornare dictum varius. Suspendisse non imperdiet metus. Mauris eget tincidunt lorem, vel consequat eros. Donec id urna ipsum. Proin ut tortor nec tellus scelerisque dapibus. Nulla tincidunt aliquet metus nec tristique. In est ante, commodo eu dui sed, viverra pharetra velit. Integer id elementum metus, finibus aliquet velit",
          )
        )
      </p>
      <p style=letterTextStyle>
        (
          str(
            "Vestibulum malesuada consequat arcu, non commodo leo vehicula in. Sed maximus vel tellus ut accumsan. Praesent ac tempor turpis, sit amet gravida nunc. Mauris vehicula ornare nulla, eget ornare odio vestibulum ut. Suspendisse eu nulla in leo mattis condimentum. Vestibulum sagittis augue ut felis tincidunt molestie. Phasellus ac neque ut urna mattis sagittis vel eget quam. Sed vitae rhoncus enim, at dignissim felis.",
          )
        )
      </p>
      <p style=letterSignatureStyle> (str("Love, David")) </p>
      <a href="/years" style=linkStyle> (ReasonReact.string("Begin")) </a>
    </div>,
};
