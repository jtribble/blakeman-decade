import React, {Component} from 'react';
import * as R from 'ramda';
import { Motion, spring } from 'react-motion';

import { getMediumImagePaths, getSmallImagePaths } from '../modules/images';


const ROW_FOCUSED_RATIO = 0.75;
const ROW_UNFOCUSED_RATIO = 0.25;
const ROW_MARGIN_RATIO = 0.03;
const IMAGE_MARGIN_RATIO = 0.02;

export const getRowFocusedHeight = windowHeight => ROW_FOCUSED_RATIO * windowHeight;
export const getRowUnfocusedHeight = windowHeight => ROW_UNFOCUSED_RATIO * windowHeight;
export const getRowMargin = windowHeight =>  ROW_MARGIN_RATIO * windowHeight;
const getImageMargin = windowHeight => IMAGE_MARGIN_RATIO * windowHeight;

export default class Row extends Component {
  static getStyles({ isFocused, style, windowHeight }) {
    return {
      display: 'flex',
      flexDirection: 'row',
      maxWidth: '100%',
      overflow: isFocused ? 'scroll' : 'hidden',
      position: 'relative',
      marginBottom: getRowMargin(windowHeight),
      marginTop: getRowMargin(windowHeight),
    };
  }

  static getRowHeight(isFocused, windowHeight) {
    return isFocused ? getRowFocusedHeight(windowHeight) : getRowUnfocusedHeight(windowHeight);
  }

  static getImageStyles() {
    return {
      height: '100%'
    };
  }

  static getImageWrapperStyles(windowHeight) {
    return {
      height: '100%',
      marginLeft: getImageMargin(windowHeight),
      marginRight: getImageMargin(windowHeight),
      width: 'auto'
    };
  }

  renderOverlay(alpha) {
    const { isFocused } = this.props;
    if (isFocused)
      return null;

    return (
      <div style={{ position: 'absolute', left: 0, background: `rgba(255, 255, 255, ${alpha})`, width: '100%', height: '100%', zIndex: 1 }} />
    );
  }

  render() {
    const { isFocused, windowHeight, year } = this.props;
    return (
      <Motion
        defaultStyle={{ alpha: isFocused ? 0 : 0.7, height: Row.getRowHeight(isFocused, windowHeight)}}
        style={{ alpha: spring(isFocused ? 0 : 0.7, {stiffness: 60, damping: 15}), height: spring(Row.getRowHeight(isFocused, windowHeight, {stiffness: 60, damping: 15}))}}
        >
        {({alpha, height}) => (
          <div style={{ height, ...Row.getStyles(this.props)}}>
            {this.renderOverlay(alpha)}
            {R.map(path => <div style={Row.getImageWrapperStyles(windowHeight)}><img key={path} src={path} style={Row.getImageStyles()} /></div>, getSmallImagePaths(year))}
          </div>
        )}
      </Motion>
    )
  }
}
