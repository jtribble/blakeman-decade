module Grid = {
  type scrollPayload = {
    .
    clientHeight: float,
    clientWidth: float,
    scrollHeight: float,
    scrollLeft: float,
    scrollTop: float,
    scrollWidth: float,
  };
  [@bs.module "react-virtualized"]
  external grid : ReasonReact.reactClass = "Grid";
  let make =
      (
        ~cellRenderer,
        ~className: string,
        ~columnCount: int,
        ~columnWidth,
        ~height: float,
        ~key: option(string)=?,
        ~rowHeight,
        ~rowCount: int,
        ~onScroll: option(scrollPayload => unit)=?,
        ~scrollLeft: option(float)=?,
        ~width,
        children,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=grid,
      ~props={
        "cellRenderer": cellRenderer,
        "className": className,
        "columnCount": columnCount,
        "columnWidth": columnWidth,
        "height": height,
        "key": key |> Js.Nullable.fromOption,
        "rowHeight": rowHeight,
        "rowCount": rowCount,
        "onScroll": onScroll |> Js.Nullable.fromOption,
        "scrollLeft": scrollLeft |> Js.Nullable.fromOption,
        "width": width,
      },
      children,
    );
};
