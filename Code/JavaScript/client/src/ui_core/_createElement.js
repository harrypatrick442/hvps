
const _createElement = (tagName, attributes = {}, ...children) => {
  const node = document.createElement(tagName);

  if (attributes) {
    Object.keys(attributes).forEach(key => {
      if (key === "className") {
        const classes = attributes[key].split(" ");
        classes.forEach(x => node.classList.add(x));
      } else if (/^data-/.test(key)) {
        const dataProp = key
          .slice(5) // removes `data-`
          .split("-")
          .map(
            (str, i) =>
              i === 0
                ? str
                : str.charAt(0).toUpperCase() + str.slice(1).toLowerCase()
          )
          .join("");
        node.dataset[dataProp] = attributes[key];
      } else {
        node.setAttribute(key, attributes[key]);
      }
    });
  }

  children.forEach(child => {
    if (typeof child === "undefined" || child === null) {
      return;
    }
    if (typeof child === "string") {
      node.appendChild(document.createTextNode(child));
    } else {
      node.appendChild(child);
    }
  });

  return node;
};
export default _createElement;