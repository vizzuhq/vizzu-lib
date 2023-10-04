const TypeDoc = require('typedoc')
const path = require('node:path')

const repoPath = path.join(__dirname, '..', '..', '..')
const toolsPath = path.join(repoPath, 'tools')
const mkdocsPath = path.join(toolsPath, 'docs')
const genPath = path.join(mkdocsPath, 'reference')
const distPath = path.join(repoPath, 'dist')

function reference() {
  const app = new TypeDoc.Application()

  app.options.addReader(new TypeDoc.TSConfigReader())

  app.bootstrap({
    plugin: ['typedoc-plugin-markdown', 'typedoc-plugin-rename-defaults'],
    entryPoints: [path.join(distPath, 'vizzu.d.ts')],
    entryPointStrategy: 'expand',
    tsconfig: path.join(genPath, 'tsconfig.json'),
    name: 'Vizzu',
    hideInPageTOC: true,
    disableSources: true,
    readme: path.join(repoPath, 'docs', 'reference', 'index.md')
  })

  const project = app.convert()

  if (project) {
    const outputDir = path.join(genPath, 'tmp')
    return app.generateDocs(project, outputDir)
  }
}

reference()
