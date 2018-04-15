import React, {Component} from 'react';
import * as R from 'ramda';
import { Motion, spring } from 'react-motion';

import { getMediumImagePaths, getSmallImagePaths } from '../modules/images';

const ROW_FOCUSED_RATIO = 0.8;
const ROW_HEADER_RATIO = 0.05;
const ROW_UNFOCUSED_RATIO = 0.79;
const ROW_MARGIN_RATIO = 0.03;
const IMAGE_MARGIN_RATIO = 0.02;

export const getRowHeaderHeight = windowHeight => ROW_HEADER_RATIO * windowHeight;
export const getRowFocusedHeight = windowHeight => ROW_FOCUSED_RATIO * windowHeight;
export const getRowUnfocusedHeight = windowHeight => ROW_UNFOCUSED_RATIO * windowHeight;
export const getRowMargin = windowHeight =>  ROW_MARGIN_RATIO * windowHeight;
const getImageMargin = windowHeight => IMAGE_MARGIN_RATIO * windowHeight;

export default class Row extends Component {
  constructor(props) {
    super(props);
    this.onScroll = this.onScroll.bind(this);
    this.state = {
      curImageIndex: 0
    };
  }

  onScroll(event) {
    console.log(event);
  }

  static getStyles({ isFocused, style, windowHeight }) {
    return {
      display: 'flex',
      flexDirection: 'row',
      maxWidth: '100%',
      overflow: isFocused ? 'scroll' : 'hidden',
      position: 'relative'
    };
  }

  static getRowHeight(isFocused, windowHeight) {
    return isFocused ? getRowFocusedHeight(windowHeight) : getRowUnfocusedHeight(windowHeight);
  }

  static getImageStyles(blur) {
    return {
      filter: `blur(${blur}px)`,
      height: '100%'
    };
  }

  static getImageWrapperStyles(windowHeight) {
    return {
      boxShadow: '0px 3px 15px rgba(0,0,0,0.2)',
      height: '100%',
      marginLeft: getImageMargin(windowHeight),
      marginRight: getImageMargin(windowHeight),
      position: 'relative',
      width: 'auto'
    };
  }

  static getHeaderStyles(windowHeight, blur) {
    return {
      filter: `blur(${blur}px)`,
      fontSize: getRowHeaderHeight(windowHeight) / 1.5,
      height: getRowHeaderHeight(windowHeight),
      margin: 0,
      marginLeft: getRowMargin(windowHeight),
      padding: 0,
      paddingTop: getRowMargin(windowHeight)
    }
  }

  render() {
    const { isFocused, windowHeight, year } = this.props;
    return (
      <Motion
        defaultStyle={{ blur: isFocused ? 0 : 3.5, height: Row.getRowHeight(isFocused, windowHeight)}}
        style={{ blur: spring(isFocused ? 0 : 3.5, {stiffness: 60, damping: 15}),  height: spring(Row.getRowHeight(isFocused, windowHeight), {stiffness: 60, damping: 15})}}
        >
        {({blur, height}) => (
          <div>
            <h1 style={Row.getHeaderStyles(windowHeight, blur)}>{year}</h1>
            <div onScroll={this.onScroll} style={{ height, ...Row.getStyles(this.props)}}>
              {R.map(
                path => (
                  <div key={path} style={Row.getImageWrapperStyles(windowHeight)}>
                    <img src={path} style={Row.getImageStyles(blur)} />
                  </div>
                ),
                getSmallImagePaths(year)
               )
             }
            </div>
          </div>
        )}
      </Motion>
    )
  }
}
