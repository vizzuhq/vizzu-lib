export const canBeAccessed = (stylesheet) => {
  try {
    stylesheet.cssRules;
    return true;
  } catch (e) {
    return false;
  }
};

export const getCSSCustomProps = (pfx = "") =>
  [...document.styleSheets].filter(canBeAccessed).reduce(
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
    .map((prop) => [prop, style.getPropertyValue(prop)])
    .filter((pv) => pv[1] !== "");
};
