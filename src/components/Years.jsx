import React, {Component} from 'react';
import * as R from 'ramda';

import Row, {getRowFocusedHeight, getRowHeaderHeight, getRowUnfocusedHeight, getRowMargin} from './Row';

const YEARS = ['2008', '2009', '2010', '2011', '2012'];

const getFocusedIndex = (windowHeight, scrollY) => R.pipe(
  () => [
    scrollY + windowHeight / 2,
    getRowMargin(windowHeight) + getRowUnfocusedHeight(windowHeight) + getRowHeaderHeight(windowHeight),
    getRowMargin(windowHeight) + getRowFocusedHeight(windowHeight) + getRowHeaderHeight(windowHeight)
  ],
  ([middleY, totalUnfocusedHeight, totalFocusedHeight]) => R.reduce(
    ([sum, index], _) => middleY < sum || sum === -1
      ? [-1, index]
      : [sum + totalUnfocusedHeight, index + 1],
    [totalFocusedHeight / 1, 0],
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

  render() {
    const { scrollY, windowHeight, windowWidth } = this.state;
    return (
      <div>
        {R.map(
          year =>
            <Row
              isFocused={getIsFocused(windowHeight, scrollY, year)}
              windowHeight={windowHeight}
              year={year}
              />,
          YEARS
        )}
      </div>
    );
  }
}
