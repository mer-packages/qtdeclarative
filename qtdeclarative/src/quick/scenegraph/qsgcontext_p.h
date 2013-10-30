/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSGCONTEXT_H
#define QSGCONTEXT_H

#include <QtCore/QObject>
#include <QtCore/qabstractanimation.h>

#include <QtGui/QImage>
#include <QtGui/QSurfaceFormat>

#include <private/qtquickglobal_p.h>
#include <private/qrawfont_p.h>

#include <QtQuick/qsgnode.h>
#include <QtQuick/private/qsgdepthstencilbuffer_p.h>

QT_BEGIN_NAMESPACE


class QSGContextPrivate;
class QSGRectangleNode;
class QSGImageNode;
class QSGGlyphNode;
class QSGRenderer;
class QSGDistanceFieldGlyphCache;
class QQuickWindow;
class QSGTexture;
class QSGMaterial;
class QSGMaterialShader;
class QSGMaterialType;
class QSGRenderLoop;

class QOpenGLContext;
class QOpenGLFramebufferObject;

class QQuickTextureFactory;
class QSGDistanceFieldGlyphCacheManager;
class QSGContext;


class Q_QUICK_PRIVATE_EXPORT QSGRenderContext : public QObject
{
    Q_OBJECT

public:
    explicit QSGRenderContext(QSGContext *context);
    ~QSGRenderContext();

    QOpenGLContext *openglContext() const { return m_gl; }
    QSGContext *sceneGraphContext() const { return m_sg; }

    virtual void initialize(QOpenGLContext *context);
    virtual void invalidate();

    virtual void renderNextFrame(QSGRenderer *renderer, GLuint fboId);
    
    virtual void precompileMaterials();
    QSGMaterialShader *prepareMaterial(QSGMaterial *material);

    virtual QSharedPointer<QSGDepthStencilBuffer> depthStencilBufferForFbo(QOpenGLFramebufferObject *fbo);
    QSGDepthStencilBufferManager *depthStencilBufferManager();

    virtual QSGDistanceFieldGlyphCache *distanceFieldGlyphCache(const QRawFont &font);
    QSGTexture *textureForFactory(QQuickTextureFactory *factory, QQuickWindow *window);

    virtual QSGTexture *createTexture(const QImage &image = QImage()) const;
    virtual QSGRenderer *createRenderer();

    void registerFontengineForCleanup(QFontEngine *engine);

    static QSGRenderContext *from(QOpenGLContext *context);

signals:
    void initialized();
    void invalidated();

public slots:
    void textureFactoryDestroyed(QObject *o);

private:
    QOpenGLContext *m_gl;
    QSGContext *m_sg;

    QMutex m_mutex;
    QHash<QQuickTextureFactory *, QSGTexture *> m_textures;
    QHash<QSGMaterialType *, QSGMaterialShader *> m_materials;

    QSGDepthStencilBufferManager *m_depthStencilManager;
    QSGDistanceFieldGlyphCacheManager *m_distanceFieldCacheManager;

    QSet<QFontEngine *> m_fontEnginesToClean;
};

class Q_QUICK_PRIVATE_EXPORT QSGContext : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QSGContext)

public:
    explicit QSGContext(QObject *parent = 0);
    ~QSGContext();

    virtual void renderContextInitialized(QSGRenderContext *renderContext);
    virtual void renderContextInvalidated(QSGRenderContext *renderContext);

    virtual QSGRectangleNode *createRectangleNode();
    virtual QSGImageNode *createImageNode();
    virtual QSGGlyphNode *createGlyphNode(QSGRenderContext *rc);
    virtual QSGGlyphNode *createNativeGlyphNode();
    virtual QAnimationDriver *createAnimationDriver(QObject *parent);

    virtual QSize minimumFBOSize() const;
    virtual QSurfaceFormat defaultSurfaceFormat() const;

    void setDistanceFieldEnabled(bool enabled);
    bool isDistanceFieldEnabled() const;

    static QSGContext *createDefaultContext();
    static QQuickTextureFactory *createTextureFactoryFromImage(const QImage &image);
    static QSGRenderLoop *createWindowManager();
};

QT_END_NAMESPACE

#endif // QSGCONTEXT_H
