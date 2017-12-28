import React, {Component} from 'react';
import {Link as ReactRouterLink} from 'react-router-dom'

export default class Link extends Component {
  static getStyle() {
    return {
      display: 'inline-block',
      width: '100%',
      textDecoration: 'none',
      textAlign: 'center'
    }
  }

  render() {
    const { style, ...rest } = this.props;
    return (
      <ReactRouterLink style={{ ...Link.getStyle(), ...style }} {...rest} />
    );
  }
}
