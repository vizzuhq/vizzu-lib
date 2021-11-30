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

export const propSet = (obj, path, value, overwrite) => {
  path.reduce((acc, part, idx) => {
    if (idx === path.length - 1) {
      if (overwrite || !acc[part]) {
        acc[part] = value;
      }
    } else if (!acc[part]) {
      acc[part] = {};
    }

    return acc[part];
  }, obj);
  return obj;
};

export const propGet = (obj, path) => {
  return path.reduce((acc, part) => acc?.[part], obj);
};

export const propsToObject = (props, propObj, pfx = "", overwrite = false) => {
  propObj = propObj || {};
  propObj = props.reduce((obj, [prop, val]) => {
    const propname = prop.replace("--" + (pfx ? pfx + "-" : ""), "");
    const proppath = propname.split("-");

    propSet(obj, proppath, val, overwrite);

    return obj;
  }, propObj);

  return propObj;
};
