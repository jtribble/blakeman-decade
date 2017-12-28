import React, {Component} from 'react';
import * as R from 'ramda';
import {getSmallImagePaths} from '../modules/images';

const getGridTemplateColumns = R.pipe(
  R.times(() => 'auto'),
  R.join(' ')
);

export default class ImageGrid extends Component {
  static getStyle({columns}) {
    return {
      display: 'grid',
      gridGap: 20,
      gridTemplateColumns: getGridTemplateColumns(columns)
    }
  }

  render() {
    return (
      <div style={ImageGrid.getStyle(this.props)}>
        {
          R.map(path => <img key={path} src={path} style={{ width: 200 }} />, getSmallImagePaths(this.props.year))
        }
      </div>
    )
  }
}
