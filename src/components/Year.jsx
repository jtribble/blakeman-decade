import React, {Component} from 'react';
import * as R from 'ramda';

import ImageGrid from './ImageGrid';
import Link from './Link'

const getNextLink = R.pipe(
  Number,
  R.cond([
    [R.lte(2017), () => ['/end', 'End']],
    [R.T, year => [`/year/${year + 1}`, String(year + 1)]]
  ]),
  ([to, text]) => <Link to={to}>{text}</Link>
);

export default class Year extends Component {
  static getStyle() {
    return {
      maxWidth: 600,
      position: 'absolute',
    };
  }

  render() {
    const { match: { params: { year } } } = this.props;
    return (
      <div style={Year.getStyle()}>
        <h1>{year}</h1>
        <ImageGrid columns={4} year={year} />
        {getNextLink(year)}
      </div>
    );
  }
}
