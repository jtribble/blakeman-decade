import React, { Component } from "react";
import * as R from "ramda";
import { AutoSizer, Grid } from "react-virtualized";

import { getValidYears, getSmallImagePaths } from "../modules/images";

const YEARS = getValidYears();

const cellRenderer = ({ rowIndex, onHover }) => ({
  columnIndex,
  key,
  style
}) => (
  <div style={{ ...style }}>
    <div
      onMouseOver={onHover}
      style={{
        boxShadow: "0px 3px 15px rgba(0,0,0,0.2)",
        height: "100%",
        position: "relative",
        width: "auto"
      }}
    >
      <img
        src={getSmallImagePaths(YEARS[rowIndex])[columnIndex]}
        style={{ height: "100%" }}
      />
    </div>
  </div>
);

export default class Years extends Component {
  constructor(props) {
    super(props);
    this.setWindowSize = this.setWindowSize.bind(this);
    this.setScrollY = this.setScrollY.bind(this);
    const context = new AudioContext();
    const oscillator = context.createOscillator();
    const g = context.createGain();
    oscillator.connect(g);
    g.connect(context.destination);
    this.state = {
      focusedRowIndex: 0,
      oscillator,
      scrollLeftByYear: R.reduce(
        (acc, year) => ({ [year]: 0, ...acc }),
        {},
        YEARS
      ),
      scrollY: window.pageYOffset,
      windowHeight: window.innerHeight,
      windowWidth: window.innerWidth
    };
  }

  componentDidMount() {
    window.addEventListener("resize", () => this.setWindowSize());
    window.addEventListener("scroll", () => this.setScrollY());
    this.state.oscillator.start();
  }

  componentDidUpdate(_, { focusedRowIndex: prevFocusedRowIndex }) {
    if (this.state.focusedRowIndex !== prevFocusedRowIndex) {
      this.state.oscillator.frequency.value = this._getOscillatorFrequency();
    }
  }

  setWindowSize() {
    this.setState({
      windowHeight: window.innerHeight,
      windowWidth: window.innerWidth
    });
  }

  setScrollY() {
    this.setState({
      scrollY: window.pageYOffset
    });
  }

  _getOscillatorFrequency() {
    return this.state.focusedRowIndex * 20 + 200;
  }

  _rowRenderer(windowWidth, { columnIndex, key, rowIndex, style }) {
    return (
      <div style={{ ...style }}>
        <Grid
          cellRenderer={cellRenderer({
            rowIndex,
            onHover: () => this.setState({ focusedRowIndex: rowIndex })
          })}
          className={`year-${key}`}
          columnWidth={this._getColumnWidth}
          columnCount={getSmallImagePaths(YEARS[rowIndex]).length}
          height={500}
          key={`grid-${key}`}
          rowHeight={500}
          rowCount={1}
          onScroll={({ scrollLeft }) =>
            this.setState({
              scrollLeftByYear: {
                ...this.state.scrollLeftByYear,
                [YEARS[rowIndex]]: scrollLeft
              }
            })
          }
          scrollLeft={this.state.scrollLeftByYear[YEARS[rowIndex]]}
          width={windowWidth}
        />
      </div>
    );
  }

  _getColumnWidth({ index }) {
    // TODO
    return 750;
  }

  render() {
    const { scrollY, windowHeight, windowWidth } = this.state;
    return (
      <Grid
        cellRenderer={props => this._rowRenderer(windowWidth, props)}
        className="years"
        columnWidth={windowWidth}
        columnCount={1}
        height={windowHeight}
        rowHeight={500}
        rowCount={YEARS.length}
        width={windowWidth}
      />
    );
  }
}
