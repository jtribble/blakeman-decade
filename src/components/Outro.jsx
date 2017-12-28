import React, {Component} from 'react';
import Link from './Link';
import {
  getLetterTextStyle,
  getLetterStyle,
  getLetterTitleStyle,
} from '../constants/styles';

export default class Outro extends Component {
  render() {
    return (
      <div style={getLetterStyle()}>
        <h1 style={getLetterTitleStyle()}>{'So it goes'}</h1>
        <p style={getLetterTextStyle()}>{'Nullam finibus lacinia dolor, nec auctor dui tincidunt ornare. Phasellus in urna eget eros ornare mattis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Donec interdum mattis scelerisque. Integer massa nisl, fringilla vel magna eu, eleifend dignissim erat. Vivamus non ligula non ipsum gravida suscipit. Proin eu mattis odio. Mauris ut urna id nunc elementum commodo. Suspendisse in turpis efficitur, mollis sem non, eleifend lorem. Aenean turpis velit, consequat a turpis a, imperdiet mattis'}</p>
        <p style={getLetterTextStyle()}>{'Nullam dapibus quam ac massa varius eleifend. Integer faucibus non mi nec imperdiet. Etiam aliquet elementum elit, sed fermentum elit feugiat eu. Etiam eu ex quis neque varius condimentum quis quis arcu. Sed ornare dictum varius. Suspendisse non imperdiet metus. Mauris eget tincidunt lorem, vel consequat eros. Donec id urna ipsum. Proin ut tortor nec tellus scelerisque dapibus. Nulla tincidunt aliquet metus nec tristique. In est ante, commodo eu dui sed, viverra pharetra velit. Integer id elementum metus, finibus aliquet velit'}</p>
        <Link to='/'>Home</Link>
      </div>
    );
  }
}
