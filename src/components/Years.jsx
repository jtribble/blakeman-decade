import React, {Component} from 'react';
import * as R from 'ramda';

import Row from './Row';

const ROW_FOCUSED_RATIO = 0.85;
const ROW_UNFOCUSED_RATIO = 0.25;
const ROW_MARGIN_RATIO = 0.03;

const YEARS = ['2008', '2009', '2010', '2011', '2012'];

const getRowFocusedHeight = windowHeight => ROW_FOCUSED_RATIO * windowHeight;
const getRowUnfocusedHeight = windowHeight => ROW_UNFOCUSED_RATIO * windowHeight;
const getRowMargin = windowHeight =>  ROW_MARGIN_RATIO * windowHeight;

const getFocusedIndex = (windowHeight, scrollY) => R.pipe(
  () => [
    scrollY + windowHeight / 2,
    getRowMargin(windowHeight) * 2 + getRowUnfocusedHeight(windowHeight),
    getRowMargin(windowHeight) * 2 + getRowFocusedHeight(windowHeight)
  ],
  ([middleY, totalUnfocusedHeight, totalFocusedHeight]) => R.reduce(
    ([sum, index], _) => middleY < sum || sum === -1
      ? [-1, index]
      : [sum + totalUnfocusedHeight, index + 1],
    [totalFocusedHeight / 1.5, 0],
    YEARS
  ),
  ([_, index]) => R.min(index, R.length(YEARS) - 1)
)();

const getIsFocused = (windowHeight, scrollY, year) =>
  YEARS[getFocusedIndex(windowHeight, scrollY)] === year;

export default class Years extends Component {
  constructor(props) {
    super(props)
    this.setWindowSize = this.setWindowSize.bind(this)
    this.setScrollY = this.setScrollY.bind(this)
    this.state = {
      scrollY: window.pageYOffset,
      windowHeight: window.innerHeight,
      windowWidth: window.innerWidth
    }
  }

  componentDidMount() {
    window.addEventListener('resize', () => this.setWindowSize());
    window.addEventListener('scroll', () => this.setScrollY());
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

  static getRowStyles(windowHeight, scrollY, year) {
    return {
      height: getIsFocused(windowHeight, scrollY, year) ? getRowFocusedHeight(windowHeight) : getRowUnfocusedHeight(windowHeight),
      marginBottom: getRowMargin(windowHeight),
      marginTop: getRowMargin(windowHeight)
    };
  }

  render() {
    const { scrollY, windowHeight, windowWidth } = this.state;
    return (
      <div>
        {R.map(
          year =>
            <Row
              isFocused={getIsFocused(windowHeight, scrollY, year)}
              style={Years.getRowStyles(windowHeight, scrollY, year)}
              windowHeight={windowHeight}
              year={year}
              />,
          YEARS
        )}
        <div style={{ height: getRowMargin(windowHeight) }} />
      </div>
    );
  }
}
