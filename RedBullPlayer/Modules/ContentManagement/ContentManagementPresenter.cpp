/*
 *  Red Bull Media Player
 *  Copyright (C) 2011, Red Bull
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ContentManagementPresenter.h"

#include "../../Container/Constants.h"
#include "../../ToolLib/ConfigManager.h"

using namespace RedBullPlayer::Modules::ContentManagement;
using namespace RedBullPlayer::Container;
using namespace RedBullPlayer::Tools;
using namespace RedBullPlayer::Modules::StyleFactory;

ContentManagementPresenter::ContentManagementPresenter( ContentManagementViewFactory *viewFactory,
        ContentManagementUI *view,
        IStyleFactory *styleFactory, QObject *parent )
    : QObject( parent ) {
    _view = view;
    _view->hide();
    _viewFactory = viewFactory;
    _styleFactory = styleFactory;
    bootstrapView();
    bootstrapTabs();
}

void ContentManagementPresenter::bootstrapView() {
    connect( _view, SIGNAL( mouseEnter() ), this, SLOT( mouseEnterView() ) );
    connect( _view, SIGNAL( mouseLeave() ), this, SLOT( mouseLeaveView() ) );
    _viewTimer = new QTimer( this );
    _viewTimer->setSingleShot( true );
    connect( _viewTimer, SIGNAL( timeout() ), this, SLOT( viewTimerTimeout() ) );
    _viewTimer->setInterval( Constants::STATUSDISPLAY_HOVERBOX_HIDE_TIMEOUT );
}

void ContentManagementPresenter::bootstrapTabs() {
    _view->setTabTitles( ConfigManager::instance()->newDownloadsTabTitle(),
                         ConfigManager::instance()->downloadedTabTitle(),
                         ConfigManager::instance()->skippedDownloadsTabTitle() );

    if( _styleFactory != NULL ) {
        bool ok;
        ITextStyle *tabTitleStyle = _styleFactory->getTextStyle( "ContentManagement_Tab_Title", &ok );

        if( ok )
            _view->setTabTitlesStyle( tabTitleStyle );
    }

    IContentListView *availableListView = _viewFactory->createNewDownloadsListView();
    _view->addAvailableListView( availableListView );
    IContentListView *downloadedListView = _viewFactory->createDownloadedListView();
    _view->addDownloadedListView( downloadedListView );
    IContentListView *skippedListView = _viewFactory->createSkippedListView();
    _view->addSkippedListView( skippedListView );
    _model = new ContentManagementModel( _viewFactory, availableListView, downloadedListView,
                                         skippedListView, this );
    _eventBridge = new ContentManagementEventBridge( _model, this );
}

void ContentManagementPresenter::viewTimerTimeout() {
    _view->hide();
}

void ContentManagementPresenter::mouseEnterView() {
    if ( _viewTimer->isActive() )
        _viewTimer->stop();
}

void ContentManagementPresenter::mouseLeaveView() {
    if ( !_viewTimer->isActive() )
        _viewTimer->start();
}

void ContentManagementPresenter::showView( int x, int y ) {
    _view->show( x, y );
    _viewTimer->start();
}

void ContentManagementPresenter::hideView() {
    _view->hide();
}

bool ContentManagementPresenter::isViewVisible() {
    return _view->isVisible();
}

