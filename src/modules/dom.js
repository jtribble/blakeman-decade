/**
 * DOM utilities.
 */

export function addDivToBody(attributes) {
  const div = document.createElement('div');
  if (attributes) {
    Object.keys(attributes).forEach(attribute => {
      div.setAttribute(attribute, attributes[attribute]);
    });
  }
  document.body.appendChild(div);
  return div;
}

export function addLinkToHead(href) {
  return new Promise((resolve, reject) => {
    const link = document.createElement('link');
    link.setAttribute('href', href);
    link.setAttribute('rel', 'stylesheet');
    link.onload = resolve;
    link.onerror = reject;
    document.head.appendChild(link);
  });
}
