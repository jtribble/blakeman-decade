module Grid = {
  [@bs.module "react-virtualized"]
  external grid : ReasonReact.reactClass = "Grid";
  let make =
      (
        ~cellRenderer,
        ~className: string,
        ~columnCount: int,
        ~columnWidth,
        ~height: float,
        ~key: string,
        ~rowHeight,
        ~rowCount: int,
        ~onScroll,
        ~scrollLeft,
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
        "key": key,
        "rowHeight": rowHeight,
        "rowCount": rowCount,
        "onScroll": onScroll,
        "scrollLeft": scrollLeft,
        "width": width,
      },
      children,
    );
};
