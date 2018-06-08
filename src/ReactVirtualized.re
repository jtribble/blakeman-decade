module Grid = {
  type scrollPayload = {
    .
    "clientHeight": float,
    "clientWidth": float,
    "scrollHeight": float,
    "scrollLeft": float,
    "scrollTop": float,
    "scrollWidth": float,
  };
  [@bs.module "react-virtualized"]
  external grid : ReasonReact.reactClass = "Grid";
  let make =
      (
        ~cellRenderer:
           {
             ..
             "columnIndex": int,
             "key": string,
             "style": ReactDOMRe.style,
           } =>
           ReasonReact.reactElement,
        ~className: string,
        ~columnCount: int,
        ~columnWidth,
        ~containerStyle: option(ReactDOMRe.Style.t)=?,
        ~height: float,
        ~key: option(string)=?,
        ~rowHeight,
        ~rowCount: int,
        ~onScroll: option(scrollPayload => unit)=?,
        ~scrollLeft: option(float)=?,
        ~scrollToAlignment: option(string)=?,
        ~style: option(ReactDOMRe.Style.t)=?,
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
        "containerStyle": containerStyle |> Js.Nullable.fromOption,
        "height": height,
        "key": key |> Js.Nullable.fromOption,
        "rowHeight": rowHeight,
        "rowCount": rowCount,
        "onScroll": onScroll |> Js.Nullable.fromOption,
        "scrollLeft": scrollLeft |> Js.Nullable.fromOption,
        "scrollToAlignment": scrollToAlignment |> Js.Nullable.fromOption,
        "style": style |> Js.Nullable.fromOption,
        "width": width,
      },
      children,
    );
  [@bs.send]
  external scrollToCell :
    (
      ReasonReact.reactRef,
      {
        .
        "columnIndex": int,
        "rowIndex": int,
      }
    ) =>
    unit =
    "";
  [@bs.send]
  external scrollToPosition :
    (
      ReasonReact.reactRef,
      {
        .
        "scrollLeft": float,
        "scrollTop": float,
      }
    ) =>
    unit =
    "";
};
