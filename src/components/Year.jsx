import React, {Component} from 'react';

export default class Year extends Component {
  static getStyle() {
    return {
      fontSize: 50,
      left: '50%',
      maxWidth: 600,
      position: 'absolute',
      top: '50%',
      transform: 'translate(-50%, -50%)'
    };
  }

  render() {
    const { match: { params: { year } } } = this.props;
    return (
      <div style={Year.getStyle()}>
        {year}
      </div>
    );
  }
}
