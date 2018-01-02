import React, {Component} from 'react';
import Link from './Link';
import {
  getLetterSignatureStyle,
  getLetterStyle,
  getLetterTextStyle,
  getLetterTitleStyle
} from '../constants/styles';

export default class Intro extends Component {
  render() {
    return (
      <div style={getLetterStyle()}>
        <h1 style={getLetterTitleStyle()}>{'Together Through Life'}</h1>
        <p style={getLetterTextStyle()}>{'Nullam finibus lacinia dolor, nec auctor dui tincidunt ornare. Phasellus in urna eget eros ornare mattis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Donec interdum mattis scelerisque. Integer massa nisl, fringilla vel magna eu, eleifend dignissim erat. Vivamus non ligula non ipsum gravida suscipit. Proin eu mattis odio. Mauris ut urna id nunc elementum commodo. Suspendisse in turpis efficitur, mollis sem non, eleifend lorem. Aenean turpis velit, consequat a turpis a, imperdiet mattis'}</p>
        <p style={getLetterTextStyle()}>{'Nullam dapibus quam ac massa varius eleifend. Integer faucibus non mi nec imperdiet. Etiam aliquet elementum elit, sed fermentum elit feugiat eu. Etiam eu ex quis neque varius condimentum quis quis arcu. Sed ornare dictum varius. Suspendisse non imperdiet metus. Mauris eget tincidunt lorem, vel consequat eros. Donec id urna ipsum. Proin ut tortor nec tellus scelerisque dapibus. Nulla tincidunt aliquet metus nec tristique. In est ante, commodo eu dui sed, viverra pharetra velit. Integer id elementum metus, finibus aliquet velit'}</p>
        <p style={getLetterTextStyle()}>{'Vestibulum malesuada consequat arcu, non commodo leo vehicula in. Sed maximus vel tellus ut accumsan. Praesent ac tempor turpis, sit amet gravida nunc. Mauris vehicula ornare nulla, eget ornare odio vestibulum ut. Suspendisse eu nulla in leo mattis condimentum. Vestibulum sagittis augue ut felis tincidunt molestie. Phasellus ac neque ut urna mattis sagittis vel eget quam. Sed vitae rhoncus enim, at dignissim felis.'}</p>
        <p style={getLetterSignatureStyle()}>{'Love, David'}</p>
        <Link to="/years">Begin</Link>
      </div>
    );
  }
}
