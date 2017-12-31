import React, {Component} from 'react';
import * as R from 'ramda';

import { getMediumImagePaths, getSmallImagePaths } from '../modules/images';

const IMAGE_MARGIN_RATIO = 0.02;
const getImageMargin = windowHeight => IMAGE_MARGIN_RATIO * windowHeight;

export default class Row extends Component {
  static getStyles({ isFocused, style }) {
    return {
      display: 'flex',
      flexDirection: 'row',
      maxWidth: '100%',
      overflow: isFocused ? 'scroll' : 'hidden',
      position: 'relative',
      ...style
    };
  }

  static getImageStyles(windowHeight) {
    return {
      height: '100%',
      marginLeft: getImageMargin(windowHeight),
      marginRight: getImageMargin(windowHeight)
    };
  }

  renderOverlay() {
    const { isFocused } = this.props;
    if (isFocused)
      return null;

    return (
      <div style={{ position: 'absolute', left: 0, background: 'rgba(255, 255, 255, 0.7)', width: '100%', height: '100%', zIndex: 1 }} />
    );
  }

  render() {
    const { isFocused, windowHeight, year } = this.props;
    return (
      <div style={Row.getStyles(this.props)}>
        {this.renderOverlay()}
        {R.map(path => <img key={path} src={path} style={Row.getImageStyles(windowHeight)} />, isFocused ? getMediumImagePaths(year) : getSmallImagePaths(year))}
      </div>
    )
  }
}
