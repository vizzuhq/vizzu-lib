export const isAccessibleStylesheet = (stylesheet) => {
  try {
    stylesheet.cssRules;
    return true;
  } catch (e) {
    return false;
  }
};

export const getCSSCustomProps = (pfx = "") =>
  [...document.styleSheets].filter(isAccessibleStylesheet).reduce(
    (finalArr, sheet) =>
      finalArr.concat(
        [...sheet.cssRules]
          .filter((rule) => rule.type === 1)
          .reduce((propValArr, rule) => {
            const props = [...rule.style]
              .filter((propName) => propName.trim().indexOf("--" + pfx) === 0)
              .map((propName) => propName.trim());
            return [...propValArr, ...props];
          }, [])
      ),
    []
  );

export const getCSSCustomPropsForElement = (el, pfx = "") => {
  const props = getCSSCustomProps(pfx);
  const style = getComputedStyle(el);
  return props
    .map((prop) => [prop, style.getPropertyValue(prop).trim()])
    .filter((pv) => pv[1] !== "");
};

export const propSet = (obj, path, value) => {
  path.reduce((acc, part, idx) => {
    if (!acc?.[part]) {
      acc[part] = idx === path.length - 1 ? value : {};
    }

    return acc[part];
  }, obj);
  return obj;
};

export const propGet = (obj, path) => {
  return path.reduce((acc, part) => acc?.[part], obj);
};

export const propsToObject = (props, propObj, pfx = "") => {
  propObj = propObj || {};
  propObj = props.reduce((obj, [prop, val]) => {
    const propname = prop.replace("--" + (pfx ? pfx + "-" : ""), "");
    const proppath = propname.split("-");

    propSet(obj, proppath, val);

    return obj;
  }, propObj);

  return propObj;
};
